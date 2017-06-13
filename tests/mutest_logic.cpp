#include "catch.hpp"

#include <iostream>

#include "Arduino.h"
#include "mock_stimer.h"
#include "config.h"
#include "logic.h"
#include "mock_logger.h"
#include "device.h"

// __UNITTEST__SOURCES_ = logic.cpp, device.cpp, stimer.cpp


class TEST_Device : public Device_output
{
   public:
   TEST_Device() : Device_output("testdev") { set_value_sum = 0; set_value_now = 0; };
   virtual void set_value( int value, bool verbose=false ) override { set_value_now = value; } 
   void loop_second() { set_value_sum += set_value_now; }
   int set_value_sum;
   int set_value_now;
} ; 


static Logic LOGIC;
static TEST_Device OUT_DEV;
static int WATER_LEVEL = 100;
static int WATER_SWITCH = 0;
static int MANUAL_SWITCH = 0;


void run_logic( int start_hour, int nseconds )
{
   
   // then enter active region
   millis_fake.return_val = start_hour*60*60*1000;
   for ( int seconds = 0; seconds < nseconds ; seconds ++ )
   {
      Config_run_table_time time;
      millis_fake.return_val += 1000;
      memset( &time, 0x00, sizeof(time));
      time.hour = start_hour;
      time.minute = seconds / 60;
      time.second = seconds - time.minute * 60;
      
      int water_switch_value ;
            
      
      if ( WATER_SWITCH == 0 )
      {
         water_switch_value = 0;
      }
      else if (WATER_SWITCH == 2 )
      {
         water_switch_value = 1;
      }
      else
      {
         
         if ( OUT_DEV.set_value_now == 1 )
         {
            if ( OUT_DEV.set_value_sum > 8 )
            {
               water_switch_value  = 1;
            }
            else
            {
               water_switch_value  = -1;
            }
         }
         else
         {
            water_switch_value = 0;
         }
      }
      
      
      LOGIC.run_logic( &time, &OUT_DEV, WATER_LEVEL, water_switch_value, MANUAL_SWITCH );
      OUT_DEV.loop_second();
   }
}

void check_status_is( Logger::Status value )
{
   REQUIRE( _TEST_log_status == value );
}

TEST_CASE( "Logic", "[logic]" ) 
{
   _TEST_log_hookup( true );
   RESET_FAKE(millis);
   
   LOGIC = Logic();
   OUT_DEV = TEST_Device();
   
   WATER_LEVEL = 100;
   WATER_SWITCH = 1;
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
      check_status_is( Logger::Status::RUNNING );
      int delay[2];
      bool delay_ok = LOGIC.get_measurements( delay );
      REQUIRE( delay_ok == true );
   }
   
   SECTION("water level not high")
   {
      WATER_LEVEL = -1;
      run_logic( CONFIG.runtable.active_start.hour, 120 );
      REQUIRE( OUT_DEV.set_value_sum == 0 );
      check_status_is( Logger::Status::ERROR );
   }
   
   SECTION("switch fixed high")
   {
      WATER_SWITCH = 2;
      run_logic( CONFIG.runtable.active_start.hour, 120 );
      REQUIRE( OUT_DEV.set_value_sum == 0 );
      check_status_is( Logger::Status::ERROR );
   }
   SECTION("switch not raising")
   {
      WATER_SWITCH = 0;
      run_logic( CONFIG.runtable.active_start.hour, 120 );
      REQUIRE( OUT_DEV.set_value_sum > 0 );
      REQUIRE( OUT_DEV.set_value_sum < 30 );
      check_status_is( Logger::Status::ERROR );
   }

   SECTION("manual switch, no matter what")
   {
      WATER_SWITCH = 0;
      WATER_LEVEL = -1;
      
      run_logic( 0, 20 );
      REQUIRE( OUT_DEV.set_value_sum == 0 );
      
      // manual switch on!
      MANUAL_SWITCH = 1;
      run_logic( 0, 20 );
      REQUIRE( OUT_DEV.set_value_sum == 20 );
      check_status_is( Logger::Status::RUNNING );
   }
   
}