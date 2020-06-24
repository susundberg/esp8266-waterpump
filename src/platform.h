#ifndef SUPA_PLATFORM_H
#define SUPA_PLATFORM_H

/** This file provides abstraction over the ESP platform provided stuff; wifi, I2C, STA and FS */
#include "stimer.h"

class Platform_ESP8266
{
   
public:
   void setup();
   void loop();
   
   bool connected(); // returns true if wlan has been aquired.
   unsigned int get_free_heap();
   
protected:
   bool wifi_ok;
   STimer wifi_lost_timer;
   
   void setup_wifi();
   void setup_i2c();
   void setup_ota();
   void setup_fs();
   
   void loop_wifi();
   void loop_ota();
} ;

extern Platform_ESP8266 PLATFORM;

#endif