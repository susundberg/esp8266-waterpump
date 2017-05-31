
#include "catch.hpp"

#include <iostream>
#include <string.h>

#include "Arduino.h"
#include "mock_stimer.h"
#include "device_pin_in.h"

// __UNITTEST__SOURCES_ = device_pin_in.cpp, device.cpp

static const int FILTER_LEN = 5;

TEST_CASE( "Basic device ", "[device]" ) 
{
   Device_pin_in pin("switch", 12, FILTER_LEN, false );
   ARDUINO_TEST.hookup();
   ARDUINO_TEST.pin_value[12] = 0;
   pin.setup();
   
   
   SECTION("Jsonnify is nice")
   {
      char buffer[256];
      int len = pin.jsonify( buffer, 256 );
      std::string target("{\"name\":\"switch\",\"value\":-1}");
      std::string source(buffer);
      
      REQUIRE( source.length() == len );
      REQUIRE( source == target );
   }
   
    SECTION("Filtering works")
    {
       ARDUINO_TEST.pin_value[12] = 1;
       STimer__check_fake.return_val = true;
       for ( int loop = 0; loop < FILTER_LEN/2; loop ++ )
       {
          pin.loop();
          REQUIRE( pin.get_value() == -1 );
       }
       // check that value stays good
       for ( int loop = 0; loop < FILTER_LEN; loop ++ )
       {
          pin.loop();
          REQUIRE( pin.get_value() == 1 );
       }
       // change value, we need double time for the filter to settle
       ARDUINO_TEST.pin_value[12] = 0;
       for ( int loop = 0; loop < FILTER_LEN /2 ; loop ++ )
       {
          pin.loop();
          REQUIRE( pin.get_value() == 1 );
       }
       for ( int loop = 0; loop < FILTER_LEN  ; loop ++ )
       {
          pin.loop();
          REQUIRE( pin.get_value() == -1 );
       }
       // and the value should stay again.
       for ( int loop = 0; loop < FILTER_LEN; loop ++ )
       {
          pin.loop();
          REQUIRE( pin.get_value() == 0 );
       }
    }
}   


TEST_CASE( "Inverted device ", "[device]" ) 
{
   Device_pin_in pin("switch", 12, FILTER_LEN, true );
   ARDUINO_TEST.hookup();
   ARDUINO_TEST.pin_value[12] = 0;
   pin.setup();

   SECTION("Filtering works")
   {
      ARDUINO_TEST.pin_value[12] = 1;
      STimer__check_fake.return_val = true;
      for ( int loop = 0; loop < FILTER_LEN; loop ++ )
      {
        pin.loop();
      }
      REQUIRE( pin.get_value() == 0 );
   }
}     