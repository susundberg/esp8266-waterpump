#include "catch.hpp"

#include <iostream>
#include <string.h>

#include "Arduino.h"
#include "mock_stimer.h"
#include "device_wlevel.h"


// __UNITTEST__SOURCES_ = device_wlevel.cpp, device.cpp

TEST_CASE( "Basic distance ", "[device]" ) 
{
   ARDUINO_TEST.hookup();
   Device_wlevel sensor("us", 5,10 );
   sensor.setup();
   
   ARDUINO_TEST.pin_value[10] = 500; // echo pin delay
   int trigger_sum = 0;
   
   STimer__check_fake.return_val = true;
   for ( int loop = 0; loop < 5; loop ++ )
   {
      trigger_sum += ARDUINO_TEST.pin_value[5];
      sensor.loop();
   }
   REQUIRE( sensor.get_value() == 85); // in the beginning we take directly the new value
   REQUIRE( trigger_sum == 1);
   ARDUINO_TEST.pin_value[10] = 250; // echo pin delay
   
   for ( int loop = 0; loop < 4; loop ++ )
      sensor.loop();
   REQUIRE( sensor.get_value() < 85 ); // We have filter here!
   
   for ( int loop = 0; loop < 500; loop ++ )
   {
      trigger_sum += ARDUINO_TEST.pin_value[5];
      sensor.loop();
   }
   REQUIRE( trigger_sum > 100 );
   REQUIRE( sensor.get_value() == 42  ); // some target, but the filter seems to settle at some point
   
}   
