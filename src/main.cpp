#include <ESP8266WiFi.h>

#include <Wire.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include "SparkFunHTU21D.h"
#include "email_send.h"


#include "RTClib.h"

#define MODULE_NAME "Main"

#include "config.h"
#include "logger.h"
#include "webserver.h"
#include "distance.h"


#define PIN_RTC_SCL 14
#define PIN_RTC_SDA 12
#define PIN_LED     15
#define PIN_PUMP    4
#define PIN_SWITCH  16
#define PIN_TRIGGER 2
#define PIN_ECHO    5
#define TIMEOUT_ON_WLAN_CONNECT_S 30 



Distance DISTANCE;
RTC_DS1307 RTC;
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
  LOG_INFO("RTC boot!");

  if (! RTC.isrunning()) 
  {
    LOG_INFO("RTC is NOT running!");
    
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  else
  {
     LOG_INFO("RTC is running!");
  }
}

void setup_wifi(void)
{ 
  
  WiFi.mode(WIFI_STA);
  // Connect to WiFi network
  WiFi.begin( CONFIG::wlan_sid, CONFIG::wlan_pass );
  
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
  LOG_INFO("Connected to %s - IP: %s", CONFIG::wlan_sid, WiFi.localIP().toString().c_str() );
  
}


void setup_ota() 
{
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  ArduinoOTA.setHostname( CONFIG::hostname );

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


void handle_email()
{
   EmailSettings settings = { .login = EMAIL_LOGIN, .password=EMAIL_PASSWORD, .sender = EMAIL_SENDER, .server_host = EMAIL_SERVER_HOST, .server_port = 465 };

   LOG_INFO("Sending email to %s", EMAIL_RECEIVER );
   bool ret = email_send( &settings, EMAIL_RECEIVER, EMAIL_SUBJECT, "Email message is here\n With some lines.\nCheers,\nRobot\n\n");

   char* buffer = webserver_get_buffer();   
   snprintf( buffer, WEBSERVER_MAX_RESPONSE_SIZE, "{\"status\":%d}", ret );
   WEBSERVER.send( 200, "application/json", buffer  );
   free(buffer);
}


void handle_misc()
{
   char* buffer = webserver_get_buffer();   
   
   static bool pump = false;
   pump = !pump;
   
   digitalWrite( PIN_PUMP, pump );
   int manual_switch = digitalRead( PIN_SWITCH );
   
   snprintf( buffer, WEBSERVER_MAX_RESPONSE_SIZE, "{\"switch\":%d, \"pump\":%d }", manual_switch, pump );
   WEBSERVER.send( 200, "application/json", buffer );

   free(buffer);
}

void handle_temp()
{
   char* buffer = webserver_get_buffer();   
    
   DateTime now = RTC.now();
   int humd = HUMIDITY.readHumidity();
   int temp = HUMIDITY.readTemperature();
   
   
   snprintf( buffer, WEBSERVER_MAX_RESPONSE_SIZE, "{ \"temperature\":%d,\"humidity\":%d, \"rtc\":%d }", 
             temp, humd, now.unixtime() );
   WEBSERVER.send( 200, "application/json", buffer );
   free(buffer);
}
 

void handle_sensor()
{
   char* buffer = webserver_get_buffer();   
   int distance = DISTANCE.get_distance();
   

   // LM35 produces 0.01v per C -> val(Volts) * 100 C/volts = Volts
   // times 10 to get single desimal, though the ADC circuit seems more like a joke.

/** 
 
Fitted to measurements made with my ESP

[[0.09, 0.0],
 [0.16, 51.0],
 [0.28, 179.0],
 [0.42, 309.0],
 [0.59, 471.0],
 [0.72, 596.0],
 [0.8, 676.0],
 [0.87, 758.0],
 [1.0, 870.0],
 [0.14, 35.0]]
 -> 
In [25]: numpy.polyfit( m[1:,0],m[1:,1], 1)
Out[25]: array([ 974.6595692 , -100.97829496])

value = 975 * volt - 100 
-> volt = (value + 100)/975
*/
   int analog_value = analogRead( A0 );
   unsigned long temp_long = ((unsigned long)(analog_value+CALIBRATION_ADC_OFFSET)*1000UL + (CALIBRATION_ADC_RANGE/2))/CALIBRATION_ADC_RANGE;
   int temp_degs  = temp_long / 10;
   int temp_parts = temp_long - temp_degs*10;
   
   DateTime now = RTC.now();
    
   snprintf( buffer, WEBSERVER_MAX_RESPONSE_SIZE, "{\"distance\":%d, \"temperature\":%d.%d,\"temp_raw\":%d, \"rtc\":%d }", 
             distance, temp_degs, temp_parts, analog_value, now.unixtime() );
   WEBSERVER.send( 200, "application/json", buffer );
   free(buffer);
}

void setup()
{
  LOG.setup_serial( CONFIG::hostname, 115200 );
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
  WEBSERVER.on("/get/sensor", handle_sensor );
  WEBSERVER.on("/get/i2c", handle_i2c );
  WEBSERVER.on("/get/temp", handle_temp );
  WEBSERVER.on("/get/misc", handle_misc );
  WEBSERVER.on("/get/email", handle_email);
  
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





