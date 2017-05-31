#include "catch.hpp"

#include <iostream>
#include <string.h>

#include "Arduino.h"
#include "device_pin_out.h"


TEST_CASE( "Basic device ", "[device]" )
{
   
   ARDUINO_TEST.hookup();
   ARDUINO_TEST.pin_value[12] = 0;
   
   SECTION("Inverted")
   {
      Device_pin_out pin("pump", 12, true );
      pin.setup();
      pin.set_value( 1 );
      REQUIRE( ARDUINO_TEST.pin_value[12] == 0 );
      pin.set_value( 0 );
      REQUIRE( ARDUINO_TEST.pin_value[12] == 1 );
   }
   
   SECTION("Direct")
   {
      Device_pin_out pin("pump", 12, false );
      pin.setup();
      pin.set_value( 1 );
      REQUIRE( ARDUINO_TEST.pin_value[12] == 1 );
   }
}