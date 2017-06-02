#include <ESP8266WiFi.h>

#include "config.h"

#include "serial.h"
#include "email_send.h"
#include "logger.h"
#include "webserver.h"
#include "ntp_client.h"

#include "device_rtc.h"
#include "device_wlevel.h"
#include "device_temp.h"
#include "platform.h"
#include "device_pin_in.h"
#include "device_pin_out.h"
#include "logic.h"
#include "push_thingspeak.h"

Device_rtc      DEV_RTC("rtc");
Device_wlevel   DEV_WLEVEL("water_level", PIN_TRIGGER, PIN_ECHO );
Device_pin_in   DEV_WDETECT("water_detect", PIN_WDETECT, 4, false ); // water detect switch
Device_temphum  DEV_TEMP("temperature" );
Device_temphum  DEV_HUMI("humidity" );
Device_pin_out  DEV_PUMP("pump", PIN_PUMP, 1 ); // pump inverted, since npn transistor - writing 0 will start the pump
Device_pin_in   DEV_SWITCH("switch", PIN_SWITCH, 8, true ); // manual switch
Logic           LOGIC;
Push_thingspeak PUSH;

/** Make some artificial devices for more information */
class Device_uptime : public Device_input
{
   public:
     Device_uptime() : Device_input("uptime") {};
     virtual void loop() override { this->value = millis()/1000; } ;
};

class Device_status : public Device_input
{
   public:
     Device_status() : Device_input("status") {};
     virtual void loop() override { this->value = (int)LOG.get_status(); } ;
};

Device_uptime DEV_UPTIME;
Device_status DEV_STATUS;

Device* const DEVICES[]  = {  &DEV_TEMP, &DEV_HUMI, &DEV_WLEVEL, &DEV_WDETECT, &DEV_RTC, &DEV_UPTIME, &DEV_STATUS, &DEV_PUMP, &DEV_SWITCH,   };

#define DEVICES_N (sizeof(DEVICES)/sizeof(Device*))

static  void logger_fatal_hook( const char* log_line )
{
   // if we are not connected, we are not storing the messages for now.
   if ( PLATFORM.connected() == false )
      return;
      
   int buffer_len = Logger::max_line_len + 128 ;
   int subject_len = 256;
   char* buffer  = (char*)malloc( buffer_len );
   char* subject = (char*)malloc( subject_len );
   
   memset( buffer, 0x00, buffer_len );
   memset( subject, 0x00, subject_len );
   
   // out of memory, lets skip the whole thing.
   if ( buffer == NULL || subject == NULL )
      return;
   
   snprintf( buffer, buffer_len -1, "Error on %s: %s.\n", CONFIG.hostname, log_line );
   snprintf( subject, subject_len -1, "[ESP] %s : error detected", CONFIG.hostname );
   
   email_send( &CONFIG.email, CONFIG.email.receiver, subject, buffer );
               
   free( subject );
   free( buffer );
}



static int generate_device_json(char* buffer)
{
   strcpy( buffer, "{\"dev\":[" );
   int buffer_offset = strlen(buffer);
   unsigned int loop;
   for ( loop = 0; loop < DEVICES_N; loop ++ )
   {
      Device* dev = DEVICES[loop];
      int added = dev->jsonify( buffer + buffer_offset, WEBSERVER_MAX_RESPONSE_SIZE - buffer_offset );
      
      if ( added == 0)
         break;
      
      buffer_offset += added;
      
      if ( buffer_offset + 2 >= WEBSERVER_MAX_RESPONSE_SIZE )
         break;
      
      buffer[ buffer_offset     ] = ',';
      buffer[ buffer_offset + 1 ] = 0;
      buffer_offset += 1;
   }
   
   if ( loop < DEVICES_N ) // exited with break
   {
      return 0;
   }
   else
   {
      buffer[ buffer_offset - 1 ] = ']';
      buffer[ buffer_offset  ] = '}';
      buffer[ buffer_offset + 1 ] = 0;
      return (buffer_offset + 1);
   }
}

static void handle_get_devices()
{
   char* buffer = webserver_get_buffer();
   
   if ( buffer == NULL )
      return;
   
   int blen = generate_device_json( buffer );
   
   if ( blen == 0 )
      WEBSERVER.send( 500, "application/json", "{\"error\":\"out of buffer\"}");
   else
      WEBSERVER.send( 200, "application/json", buffer );

   free( buffer );
}

static bool handle_set_email()
{
   LOG_INFO("Status email requested.");
   const int subject_len = 256;
   char* subject = (char*)malloc( subject_len );
   char* buffer = (char*)malloc(WEBSERVER_MAX_RESPONSE_SIZE);
   memset( subject, 0x00, subject_len );
   memset( buffer, 0x00, WEBSERVER_MAX_RESPONSE_SIZE);
   
   snprintf( subject, subject_len, "[ESP] %s : Status report", CONFIG.hostname );
   int blen = generate_device_json( buffer );
   serial_print_raw( buffer, blen, true );
   bool ret = email_send( &CONFIG.email, CONFIG.email.receiver, subject, buffer );
   
   free(subject);
   free(buffer);
   return ret;
}

static bool handle_set_ntp()
{
   uint32_t ntp_time = ntp_update();
   if ( ntp_time == 0 )
   {
      return false;
   }
   DEV_RTC.update_time( ntp_time );
   return true;
}

static bool handle_set_push()
{
   LOG_INFO("Data push requested.");
   bool ret = PUSH.thingspeak_push( (const Device_input**)DEVICES, 7, true );
   return ret;
}

static void handle_http( bool ret )
{
   char* buffer = webserver_get_buffer();
   if (buffer == NULL)
      return;
   
   int resp_code  = (ret == true) ? 200 : 500;
   const char* code = (ret == true) ? "ok" : "err";
   
   snprintf( buffer,  WEBSERVER_MAX_RESPONSE_SIZE, "{\"status\":\"%s\"}", code );
   WEBSERVER.send( resp_code , "application/json",  buffer );
   free(buffer);
}



void add_password_protected( const char* url, void (*handler)(void)  )
{
  
  char* buffer = (char*)malloc(1024);
  strcpy( buffer, "/set/" );
  strcat( buffer, CONFIG.password );
  strcat( buffer, "/" );
  strcat( buffer, url );
  WEBSERVER.on(  buffer , handler );
  free(buffer);
}

void setup()
{
  LOG.setup_serial( CONFIG.hostname , 115200 );
  LOG.setup_led( PIN_LED );
  LOG.setup_fatal_hook( logger_fatal_hook );
  PLATFORM.setup();
  
  webserver_setup();
  
  for ( unsigned int loop = 0; loop < DEVICES_N; loop ++ )
     DEVICES[loop]->setup();
  
  LOG.set_status( Logger::Status::RUNNING );
  
  WEBSERVER.on( "/get/dev", handle_get_devices );
  add_password_protected("ntp", []{ handle_http(handle_set_ntp()); } );
  add_password_protected("push",[]{ handle_http(handle_set_push()); });
  //out of memory: add_password_protected("email", handle_set_email );

}

void handle_serial()
{
    int line_len;
    char* line = serial_receive( &line_len );
    
    if ( line == NULL )
       return;
    
    LOG_WARN("Serial: %s ", line );

    if ( strcmp(line, "email") == 0 )
    {
       handle_set_email();
    }
    else if ( strcmp(line, "push") == 0 )
    {
       handle_set_push();
    }
    else
    {
       serial_print( "Invalid command\n");
    }
} 

void loop()
{
   
   LOG.loop();
   handle_serial();
   
   static unsigned long avail_memory_last = 0xFFFF;
   unsigned long avail_memory_now = PLATFORM.get_free_heap();
   
   if ( avail_memory_now < avail_memory_last )
   {
      LOG_INFO("Available memory dropped: %u", avail_memory_now );
      avail_memory_last = avail_memory_now;
   }
   
   
   webserver_loop();
   PLATFORM.loop();
   
   delay(10);
   
   
   for ( unsigned int loop = 0; loop < DEVICES_N; loop ++ )
     DEVICES[loop]->loop();
   
   Config_run_table_time time_now;
   DEV_RTC.time_of_day( &time_now );
   
   if ( LOG.get_status() == Logger::Status::RUNNING )
   {
      LOGIC.run_logic( &time_now, &DEV_PUMP, DEV_WLEVEL.get_value(), DEV_WDETECT.get_value(), DEV_SWITCH.get_value() );
   }
   
   PUSH.thingspeak_push( (const Device_input**)DEVICES, 7, false );
  

}





