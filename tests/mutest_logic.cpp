#include "catch.hpp"

#include <iostream>

#include "Arduino.h"
#include "mock_stimer.h"
#include "config.h"
#include "logic.h"

#include "device.h"

// __UNITTEST__SOURCES_ = logic.cpp, device.cpp


class TEST_Device : public Device_output
{
   public:
   TEST_Device() : Device_output("testdev") { set_value_sum = 0; set_value_now = -1; };
   void set_value( int value ) { set_value_sum += value; set_value_now = value; }
   
   int set_value_sum;
   int set_value_now;
} ; 

static Logic LOGIC;
static TEST_Device OUT_DEV;
static int WATER_LEVEL = 100;
static int WATER_SWITCH = 0;
static int WATER_SWITCH_OFFSET = 0;
static int MANUAL_SWITCH = 0;

void run_logic( int start_hour, int nseconds )
{
   
   // then enter active region
   for ( int seconds = 0; seconds < nseconds ; seconds ++ )
   {
      Config_run_table_time time;
      memset( &time, 0x00, sizeof(time));
      time.hour = start_hour;
      time.minute = seconds / 60;
      time.second = seconds - time.minute * 60;
      LOGIC.run_logic( &time, &OUT_DEV, WATER_LEVEL, (seconds > 5)*WATER_SWITCH + WATER_SWITCH_OFFSET , MANUAL_SWITCH );
   }
}

void check_status_is( int value )
{
   char buffer[256];
   REQUIRE( LOGIC.jsonify( buffer, 256 ) > 0 );
   
   char target_buffer[256];
   sprintf(target_buffer, "{\"name\":\"pump_status\",\"value\":%d}", value );
   
   std::string target(target_buffer);
   std::string source(buffer);
   REQUIRE( source == target );
}

TEST_CASE( "Logic", "[logic]" ) 
{
   LOGIC = Logic();
   STimer__check_fake.return_val = true;
   OUT_DEV = TEST_Device();
   
   WATER_LEVEL = 100;
   WATER_SWITCH = 1;
   WATER_SWITCH_OFFSET = 0;
   MANUAL_SWITCH = 0;
   
   SECTION("Normal running")
   {
      run_logic( 0, 120 );
      REQUIRE( OUT_DEV.set_value_sum == 0 );
      
      run_logic( CONFIG.runtable.active_start.hour, 120 );
      REQUIRE( OUT_DEV.set_value_sum == CONFIG.runtable.period_on.minute*60 );
      OUT_DEV.set_value_sum = 0;
      run_logic( CONFIG.runtable.active_stop.hour, 120 );
      REQUIRE( OUT_DEV.set_value_sum == 0 );
      check_status_is(0);
   }
   SECTION("water level not high")
   {
      WATER_LEVEL = -1;
      run_logic( CONFIG.runtable.active_start.hour, 120 );
      REQUIRE( OUT_DEV.set_value_sum == 0 );
      check_status_is(2);
   }
   SECTION("switch fixed high")
   {
      WATER_SWITCH_OFFSET = 1;
      run_logic( CONFIG.runtable.active_start.hour, 120 );
      REQUIRE( OUT_DEV.set_value_sum == 0 );
      check_status_is(2);
   }
   SECTION("switch not raising")
   {
      WATER_SWITCH = 0;
      run_logic( CONFIG.runtable.active_start.hour, 120 );
      REQUIRE( OUT_DEV.set_value_sum > 0 );
      REQUIRE( OUT_DEV.set_value_sum < 30 );
      check_status_is(2);
   }

   SECTION("manual switch, no matter what")
   {
      WATER_SWITCH = 0;
      WATER_SWITCH_OFFSET = 1;
      WATER_LEVEL = -1;
      
      run_logic( 0, 20 );
      REQUIRE( OUT_DEV.set_value_sum == 0 );
      
      // manual switch on!
      MANUAL_SWITCH = 1;
      run_logic( 0, 20 );
      REQUIRE( OUT_DEV.set_value_sum == 20 );
   }
   
}