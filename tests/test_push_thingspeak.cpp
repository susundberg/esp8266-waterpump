#include <climits>
#include "catch.hpp"
#include "Arduino.h"

#include "push_thingspeak.h"
#include "mock_device_pin_in.h"

TEST_CASE( "Basic push", "[push]" ) 
{
   
   Device_pin_in_FAKE   DEV1(1 ); 
   Device_pin_in_FAKE   DEV2(5 ); 
   Device_pin_in_FAKE   DEV3(10); 
   
   Push_thingspeak PUSH;

   Device* const DEVICES[]  = {  &DEV1, &DEV2, &DEV3 };
   
   PUSH.thingspeak_push( (const Device_input**) DEVICES, 3, true );

 
   
}