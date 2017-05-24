#include <ESP8266WiFi.h>

#include <Wire.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include "SparkFunHTU21D.h"
#include "email_send.h"




#define MODULE_NAME "Main"

#include "config.h"
#include "logger.h"
#include "webserver.h"

#include "device_rtc.h"
#include "device_wlevel.h"
#include "platform.h"



Device_rtc    DEV_RTC("rtc");
Device_wlevel DEV_WLEVEL("wlevel", PIN_TRIGGER, PIN_ECHO );
Platform_ESP8266 PLATFORM;

HTU21D HUMIDITY;

void setup_misc()
{
    pinMode( PIN_PUMP, OUTPUT);
    digitalWrite( PIN_PUMP, 1 );
    pinMode( PIN_SWITCH, INPUT);
}


void setup_humidity(void)
{
   HUMIDITY.begin();
}


void handle_temp()
{
   char* buffer = webserver_get_buffer();   
    
   int humd = HUMIDITY.readHumidity();
   int temp = HUMIDITY.readTemperature();
   
   
   snprintf( buffer, WEBSERVER_MAX_RESPONSE_SIZE, "{ \"temperature\":%d,\"humidity\":%d }", 
             temp, humd  );
   WEBSERVER.send( 200, "application/json", buffer );
   free(buffer);
}
 

void setup()
{
  LOG.setup_serial( CONFIG.hostname , 115200 );
  LOG.setup_led( PIN_LED );
  
  DEV_WLEVEL.setup();
  DEV_RTC.setup();
  

  setup_humidity();
  setup_misc();
  

  webserver_setup();
  WEBSERVER.on("/get/temp", handle_temp );
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





