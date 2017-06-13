#include <climits>
#include "catch.hpp"
#include "Arduino.h"

#include "push_thingspeak.h"
#include "WiFiClient.h"

TEST_CASE( "Basic push", "[push]" ) 
{
   
   Push_thingspeak PUSH;
   WiFiClient::_test_verbose = true;
   
   int values[6] = { 0,1,2,3,4,5 };
   int values2[3] = { 5,7,8 };
   PUSH.thingspeak_push( values,0, 6, true );
   PUSH.thingspeak_push( values2,3, 3, true );
   
}