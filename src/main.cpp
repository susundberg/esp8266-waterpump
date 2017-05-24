#include <ESP8266WiFi.h>

#include <Wire.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FS.h>

#include "email_send.h"




#define MODULE_NAME "Main"

#include "config.h"
#include "logger.h"
#include "webserver.h"

#include "device_rtc.h"
#include "device_wlevel.h"
#include "device_temp.h"
#include "platform.h"
#include "device_pin_in.h"
#include "device_pin_out.h"


Device_rtc     DEV_RTC("rtc");
Device_wlevel  DEV_WLEVEL("water_level", PIN_TRIGGER, PIN_ECHO );
Device_pin_in   DEV_WDETECT("water_detect", PIN_WDETECT, 8 ); // water detect switch
Device_temphum DEV_TEMP("temperature" );
Device_temphum DEV_HUMI("humidity" );
Device_pin_out  DEV_PUMP("pump", PIN_PUMP, 1 );
Device_pin_in   DEV_SWITCH("switch", PIN_SWITCH, 8 ); // manual switch


const Device* DEVICES[] = { &DEV_RTC, &DEV_WLEVEL, &DEV_WDETECT };

void logger_fatal_hook( const char* log_line )
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



 

void setup()
{
  LOG.setup_serial( CONFIG.hostname , 115200 );
  LOG.setup_led( PIN_LED );
  
  webserver_setup();
  LOG.set_status( Logger::Status::RUNNING );
}

void loop()
{
   LOG.loop();
   
   static unsigned long avail_memory_last = 0xFFFF;
   unsigned long avail_memory_now = ESP.getFreeHeap() ;
   
   if ( avail_memory_now < avail_memory_last )
   {
      LOG_INFO("Available memory dropped: %u", avail_memory_now );
      avail_memory_last = avail_memory_now;
   }
   
   if (LOG.get_status() != Logger::Status::RUNNING)
   {
      delay(100);
      return;
   }
   
   webserver_loop();
   
}





