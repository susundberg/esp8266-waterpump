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
#include "distance.h"

#include "device_rtc.h"

#define TIMEOUT_ON_WLAN_CONNECT_S 30 


Device_rtc DEV_RTC("rtc");

Distance DISTANCE;

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

void setup_rtc(void)
{

}

void setup_wifi(void)
{ 
  
  WiFi.mode(WIFI_STA);
  // Connect to WiFi network
  WiFi.begin( CONFIG.wlan.sid, CONFIG.wlan.pass);
  
  int rtc_done = 0;
  

  
  int loop = 0;
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    LOG_INFO("WIFI booting up..");
    loop ++ ;
    if ( loop > TIMEOUT_ON_WLAN_CONNECT_S * 10 )
    {
       LOG_WARN("WIFI connection failed!");
       return;
    }
    delay(100);
  }

    
  LOG_INFO("WIFI ok..");
  LOG_INFO("Connected to %s - IP: %s", CONFIG.wlan.sid, WiFi.localIP().toString().c_str() );
  
}


void setup_ota() 
{
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  ArduinoOTA.setHostname( CONFIG.hostname );

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

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
    LOG_ERROR("Error %u during update: ", error);
    if (error == OTA_AUTH_ERROR) LOG_ERROR("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) LOG_ERROR("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) LOG_ERROR("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) LOG_ERROR("Receive Failed");
    else if (error == OTA_END_ERROR) LOG_ERROR("End Failed");
  });
  ArduinoOTA.begin();

}

void loop_ota() 
{
  ArduinoOTA.handle();
}

static const unsigned long CALIBRATION_ADC_RANGE  = 975; // what value does the adc really give with 1.0V
static const unsigned long CALIBRATION_ADC_OFFSET = 100;

void setup_i2c()
{
   Wire.begin( PIN_RTC_SDA, PIN_RTC_SCL );
}

void handle_i2c()
{
}

/*
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
  
   WEBSERVER.send( 200, "application/json", "" );
}
*/


#define EMAIL_SENDER EMAIL_LOGIN
#define EMAIL_SERVER_HOST "smtp.gmail.com"
#define EMAIL_SUBJECT "Waterpump status"



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
  DISTANCE.setup( PIN_TRIGGER, PIN_ECHO );
  pinMode( A0, INPUT);
  
  setup_wifi();
  setup_ota();
  setup_i2c();
  setup_rtc();
  setup_humidity();
  setup_misc();
  
  bool mounted = SPIFFS.begin();
  if (mounted == true )
  {
     LOG_INFO("FS mounted.");   
  }
  else
  {
    LOG_ERROR("FS mounting failed!");    
  }

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
   
   loop_ota();
   
   if (LOG.get_status() != Logger::Status::RUNNING)
   {
      delay(100);
      return;
   }
   
   webserver_loop();
   
}





