
#include <Wire.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FS.h>

#include "logger.h"
#include "config.h"


#include "platform.h"

#define TIMEOUT_ON_WLAN_CONNECT_S 30 

Platform_ESP8266 PLATFORM;

void Platform_ESP8266::setup()
{
   this->setup_fs();
   this->setup_i2c();
   this->setup_wifi();
   this->setup_ota();
}

void Platform_ESP8266::loop()
{
   this->loop_wifi();
   this->loop_ota();
}

void Platform_ESP8266::setup_wifi(void)
{ 
  WiFi.mode(WIFI_STA);
  // Connect to WiFi network
  WiFi.begin( CONFIG.wlan.sid, CONFIG.wlan.pass);
  LOG_INFO("Wifi starting up .. ");
}

bool Platform_ESP8266::connected()
{
   return wifi_ok;
}

unsigned int Platform_ESP8266::get_free_heap()
{
   return ESP.getFreeHeap();
}


void Platform_ESP8266::loop_wifi()
{
  // Wait for connection
  if (WiFi.status() == WL_CONNECTED) 
  {
     if ( wifi_ok == false )
     {
        LOG_INFO("WIFI ok..");
        LOG_INFO("Connected to %s - IP: %s", CONFIG.wlan.sid, WiFi.localIP().toString().c_str() );
     }
     this->wifi_ok = true;
  }
  else  // it should re-connect automatically!
  {
     if ( wifi_ok == true )
     {
        LOG_WARN("WIFI connection lost!");
     }
     this->wifi_ok = false;
  }
}


void Platform_ESP8266::setup_ota() 
{
  ArduinoOTA.setHostname( CONFIG.hostname );

  // No authentication by default
  ArduinoOTA.setPassword( CONFIG.password );

  ArduinoOTA.onStart([]() 
  {
    LOG_WARN("Update start!");
    SPIFFS.end();
  });
  
  ArduinoOTA.onEnd([]() 
  {
    LOG_WARN("Update done!");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) 
  {
    static unsigned int last = 0xFF;
    unsigned int prog = (progress / (total / 100));
    
    if ( prog == last )
       return;
    
    last = prog;
    LOG_INFO("Progress: %u%%", prog );
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    LOG_FATAL("Error %u during update: ", error);
  });
  ArduinoOTA.begin();
}

void Platform_ESP8266::loop_ota()
{
   ArduinoOTA.handle();
}

void Platform_ESP8266::setup_fs()
{
  bool mounted = SPIFFS.begin();
  if (mounted == true )
  {
     LOG_INFO("FS mounted.");   
  }
  else
  {
    LOG_FATAL("FS mounting failed!");    
  }
}


void Platform_ESP8266::setup_i2c()
{
   Wire.begin( PIN_RTC_SDA, PIN_RTC_SCL );
}

