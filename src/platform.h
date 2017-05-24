#ifndef SUPA_PLATFORM_H
#define SUPA_PLATFORM_H

/** This file provides abstraction over the ESP platform provided stuff; wifi, I2C, STA and FS */

class Platform_ESP8266
{
   void setup();
   void loop();
   
   bool connected(); // returns true if wlan has been aquired.
   
protected:
   bool wifi_ok;
   
   void setup_wifi();
   void setup_i2c();
   void setup_ota();
   void setup_fs();
   
   void loop_wifi();
   void loop_ota();
} ;


#endif