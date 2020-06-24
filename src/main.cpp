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
Device_wlevel   DEV_WLEVEL("wlevel", PIN_TRIGGER, PIN_ECHO );
Device_pin_in   DEV_WDETECT("wdetect", PIN_WDETECT, 4, true ); // water detect switch - when its physically low, its open -> high
Device_temphum  DEV_TEMP("temp" );
Device_temphum  DEV_HUMI("hum" );
Device_pin_out  DEV_PUMP("pump", PIN_PUMP, 1 ); // pump inverted, since npn transistor - writing 0 will start the pump
Device_pin_in   DEV_SWITCH("switch", PIN_SWITCH, 8, true ); // manual switch
Logic           LOGIC;
Push_thingspeak PUSH;
static const int UPDATE_DELAY_S = 20 ; // Thingspeak has 15sec limit on free accounts :/
static const int FATAL_REBOOT_DELAY_S = 2250.0;

static STimer LOCAL_reset_fatal_timer;
static bool   LOCAL_reset_fatal_timer_started = false;

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

Device* const DEVICES[]  = {  &DEV_TEMP, &DEV_HUMI, &DEV_WLEVEL, &DEV_WDETECT, &DEV_PUMP, &DEV_UPTIME, &DEV_RTC, &DEV_STATUS, &DEV_SWITCH,   };

#define DEVICES_N (sizeof(DEVICES)/sizeof(Device*))

static  void logger_fatal_hook( const char* log_line )
{

   if ( LOCAL_reset_fatal_timer_started == false )
   {
      LOCAL_reset_fatal_timer.reset();
      LOCAL_reset_fatal_timer_started = true;
   }

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

static bool handle_push_devices(bool force)
{
   int values[6];
   for ( unsigned int loop = 0; loop < 6; loop ++ )
      values[loop] = DEVICES[loop]->get_value();
   return PUSH.thingspeak_push( (const int*)values, 0, 6, force );
}


#ifndef NDEBUG
static bool handle_set_email()
{
   LOG_INFO("Email requested.");
   const int subject_len = 256;
   char* subject = (char*)malloc( subject_len );
   char* buffer = (char*)malloc(WEBSERVER_MAX_RESPONSE_SIZE);
   memset( subject, 0x00, subject_len );
   memset( buffer, 0x00, WEBSERVER_MAX_RESPONSE_SIZE);
   
   snprintf( subject, subject_len, "[ESP] %s: Status", CONFIG.hostname );
   int blen = generate_device_json( buffer );
   serial_print_raw( buffer, blen, true );
   bool ret = email_send( &CONFIG.email, CONFIG.email.receiver, subject, buffer );
   
   free(subject);
   free(buffer);
   return ret;
}
#endif

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
   return handle_push_devices( true );
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

static bool handle_reboot()
{
    LOG_INFO("Reboot requested, reboot now!");
    ESP.restart();
    return true;
}

static void handle_get_time()
{
   Config_run_table_time time;
   char* buffer = webserver_get_buffer();
   if (buffer == NULL)
      return;
   
   DEV_RTC.time_of_day( &time );
   
   snprintf( buffer,  WEBSERVER_MAX_RESPONSE_SIZE, "{\"hour\":%d,\"min\":%d,\"sec\":%d}", 
             time.hour, time.minute, time.second );
   WEBSERVER.send( 200 , "application/json",  buffer );
   free(buffer);
   
}

static void add_password_protected( const char* url, void (*handler)(void)  )
{
  
  char* buffer = (char*)malloc(256);
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
  WEBSERVER.on( "/get/time", handle_get_time );
  add_password_protected("ntp", []{ handle_http( handle_set_ntp() ); } );
  add_password_protected("reboot",[]{ handle_http( handle_reboot() ); } );
}
#ifndef NDEBUG
static void handle_serial()
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
       serial_print( "Invalid\n");
    }
} 
#endif


void loop()
{
   
   LOG.loop();

   #ifndef NDEBUG
   handle_serial();
   #endif

   static STimer update_timer;
   
   static bool   update_when_elapsed = false;

   static unsigned long avail_memory_last = 0xFFFF;
   unsigned long avail_memory_now = PLATFORM.get_free_heap();
   
   if ( avail_memory_now < avail_memory_last )
   {
      LOG_INFO("Mem: %u", avail_memory_now );
      avail_memory_last = avail_memory_now;
   }
   
   
   webserver_loop();
   PLATFORM.loop();
   
   delay(10);

   
   for ( unsigned int loop = 0; loop < DEVICES_N; loop ++ )
     DEVICES[loop]->loop();
   
   Config_run_table_time time_now;
   DEV_RTC.time_of_day( &time_now );
   
   
   if ( LOG.get_status() == Logger::Status::ERROR )
   {
      if ( LOCAL_reset_fatal_timer_started && LOCAL_reset_fatal_timer.check( FATAL_REBOOT_DELAY_S*1000 ) )
      {
         LOG_INFO("Reboot timer is up, reboot now!");
         ESP.restart();
      }
   }


   if ( update_when_elapsed == true )
   {
      if ( update_timer.check( UPDATE_DELAY_S * 1000 ) == true )
      {
         update_when_elapsed = false;         
         int delays[2];
         bool valid = LOGIC.get_measurements( delays );
         if ( valid )
         {
            PUSH.thingspeak_push( (const int*)delays, 6, 2, true );
         }
         else
         {
            LOG_INFO("Delay meas failed!");
         }
      }
   }
   bool logic_changed = LOGIC.run_logic( &time_now, &DEV_PUMP, DEV_WLEVEL.get_value(), DEV_WDETECT.get_value(), DEV_SWITCH.get_value() );
   
   if ( logic_changed )
   {
      LogicStatus status = LOGIC.get_status();
      
      // if new status is idle, then we were pumping -> push the delays (assuming valid)
      if ( status == LogicStatus::idle )
      {
         update_when_elapsed = true;
         update_timer.reset();

      }
      else if ( status == LogicStatus::pump_started || status == LogicStatus::draining )
      {
         handle_push_devices( true );
      }
   }
   else
   {
      handle_push_devices( false );
   }

}





