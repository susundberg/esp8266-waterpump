#include <ESP8266WiFi.h>

#include "config.h"

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


Device_rtc      DEV_RTC("rtc");
Device_wlevel   DEV_WLEVEL("water_level", PIN_TRIGGER, PIN_ECHO );
Device_pin_in   DEV_WDETECT("water_detect", PIN_WDETECT, 8 ); // water detect switch
Device_temphum  DEV_TEMP("temperature" );
Device_temphum  DEV_HUMI("humidity" );
Device_pin_out  DEV_PUMP("pump", PIN_PUMP, 1 );
Device_pin_in   DEV_SWITCH("switch", PIN_SWITCH, 8 ); // manual switch


Device* const DEVICES[]  = { &DEV_PUMP, &DEV_SWITCH, &DEV_RTC, &DEV_WLEVEL, &DEV_WDETECT, &DEV_TEMP, &DEV_HUMI,  };

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
   
   snprintf( buffer, buffer_len, "Error on %s: %s.\n", CONFIG.hostname, log_line );
   snprintf( subject, subject_len, "[ESP] %s : error detected", CONFIG.hostname );
   
   email_send( &CONFIG.email, CONFIG.email.receiver, subject, buffer );
               
   free( subject );
   free( buffer );
}



 
static void handle_get_devices()
{
   char* buffer = webserver_get_buffer();
   int buffer_offset = 0;
   
   if ( buffer == NULL )
      return;
   
   buffer[0] = '{';
   buffer[1] = 0;
   buffer_offset = 1;
   
   unsigned int loop;
   for ( loop = 0; loop < DEVICES_N; loop ++ )
   {
      Device* dev = DEVICES[loop];
      int added = dev->jsonify( buffer + buffer_offset, WEBSERVER_MAX_RESPONSE_SIZE - buffer_offset );
      
      if ( added == 0)
         break;
      
      buffer_offset += added;
      
      if ( buffer_offset + 1 >= WEBSERVER_MAX_RESPONSE_SIZE )
         break;
      
      buffer[ buffer_offset     ] = ',';
      buffer[ buffer_offset + 1 ] = 0;
      buffer_offset += 1;
   }
   
   if ( loop < DEVICES_N ) // exited with break
   {
      WEBSERVER.send( 500, "application/json", "{\"error\":\"out of buffer\"}");
   }
   else
   {
      buffer[ buffer_offset - 1 ] = '}';
      WEBSERVER.send( 200, "application/json", buffer );
   }
   free( buffer );
}

static void handle_set_ntp()
{
   LOG_INFO("NTP time requested.");
   uint32_t ntp_time = ntp_update();
   
   if ( ntp_time == 0 )
   {
      WEBSERVER.send( 500, "application/json", "{\"error\":\"update failed\"}" );
      return;
   }
   
   DEV_RTC.update_time( ntp_time );
   char* buffer = webserver_get_buffer();
   snprintf( buffer, WEBSERVER_MAX_RESPONSE_SIZE, "{\"time_now\":%u}", (unsigned int)ntp_time );
   WEBSERVER.send( 200, "application/json", buffer );
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
  
  WEBSERVER.on( "get/devices", handle_get_devices );
  
  char* buffer = (char*)malloc(1024);
  strcpy( buffer, "set/" );
  strcat( buffer, CONFIG.password );
  strcat( buffer, "/ntp" );
  WEBSERVER.on(  buffer , handle_set_ntp );
  free(buffer);
}

void loop()
{
   
   LOG.loop();
   
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
   
   if ( LOG.get_status() != Logger::Status::RUNNING )
      return;
   
   for ( unsigned int loop = 0; loop < DEVICES_N; loop ++ )
     DEVICES[loop]->loop();
   
}





