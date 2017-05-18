#ifndef FAKE_ESP8266WIFI_H
#define FAKE_ESP8266WIFI_H

#include "ESP8266WiFiType.h"
#include "Arduino.h"

class ESP8266WiFiClass 
{
public:
        bool mode(WiFiMode_t);
        wl_status_t begin( const char* ssid, const char* password );
        wl_status_t status();
        IPAddress localIP();
};

extern ESP8266WiFiClass WiFi;

class EspClass {
public:
   uint32_t getFreeHeap();
};
extern EspClass ESP;


#endif