
#include <limits.h>

#include "logic.h"
#include "logger.h"


Logic::Logic() 
{
   status = LogicStatus::idle;
   pump_timer.reset();
   poll_timer.reset();
   measure_reset();
}


void Logic::measure_reset()
{
   measure_delay_p1_d1 = 0;
   measure_delay_p0_d0 = 0;
}

LogicStatus Logic::get_status()
{
   return status;
}

bool Logic::get_measurements(int* delays)
{
   LOG_INFO("PLogic meas %u %u", measure_delay_p0_d0, measure_delay_p1_d1 );
   
   if ( measure_delay_p0_d0 <= 0 || measure_delay_p1_d1 <= 0 )
   {
      return false;
   }
   #define CLAMP_TO_INT_MAX(x) ( x < INT_MAX ) ? x : INT_MAX ;
   
   delays[0] = CLAMP_TO_INT_MAX( measure_delay_p0_d0 );
   delays[1] = CLAMP_TO_INT_MAX( measure_delay_p1_d1 );
   return true;
}


static int time_to_secs( const Config_run_table_time* time )
{
   return (time->hour*60 + time->minute)*60 + time->second;
}

void Logic::loop_measurement( int water_switch )
{
   // Do the measurements outside of the poll-interval loop
   if ( status == LogicStatus::pump_started )
   {
      if ( measure_delay_p1_d1 <= 0 && water_switch == 1 )
      {
         measure_delay_p1_d1 = pump_timer.elapsed();
      }
   }
   else if ( status == LogicStatus::draining )
   {
      if ( measure_delay_p0_d0 <= 0 && water_switch == 0 )
      {
         measure_delay_p0_d0 = pump_timer.elapsed();
      }
   }
}


bool Logic::check_timer_safety_ok( int pump,  int water_level, int water_switch )
{
   if ( pump == 0 )
   {
      pump_start_check_done = false;
      return true;
   }
     
   if ( pump_start_check_done == false )
   {
         // rule 1: we must have water level in high enough (when starting). When pump is running the level will drop.
         if ( water_level <= 0 )
         {
            LOG_FATAL("Water level too low (%d), pump not started.", water_level );
            return false;
         }
         
         // rule 2: the water switch must be in neutral position (or its stuck)
         if ( water_switch != 0 )
         {
            LOG_FATAL("Water switch still active, pump not started.");
            return false;
         }
         pump_start_check_done = true;
         measure_reset();
         return true;
   }
   else
   {
      if ( pump_timer.check( CONFIG.pump.threshold_water_up_s * 1000 ) == true )
      {
        if ( water_switch != 1 )
        {
            LOG_FATAL("Water switch not active, even pump running for the Time.");
            return false;
        }
      }
      return true;
   }
}



int Logic::loop_timer( const Config_run_table_time* time_now )
{
   
   // If we are on error mode, do not pump.
   if ( LOG.get_status() != Logger::Status::RUNNING )
      return 0;
   
   int time_start_s = time_to_secs( &CONFIG.runtable.active_start );
   int time_stop_s  = time_to_secs( &CONFIG.runtable.active_stop);
   int time_now_s   = time_to_secs( time_now );
   
   // ok, not driving with manual mode, check if we are running on timer
   // active time!
   if ( time_start_s <= time_now_s && time_now_s < time_stop_s )
   {
      
      int period_on_s  = time_to_secs( &CONFIG.runtable.period_on );
      int period_off_s = time_to_secs( &CONFIG.runtable.period_off );
      int period_full  = period_on_s + period_off_s;
      int period_time_s = time_now_s - time_start_s;
      
      int period_n = period_time_s / period_full;
      int period_rel = period_time_s - period_n*period_full;
        
      bool pump_should_be_on = period_rel <  period_on_s;
      
      if (pump_should_be_on == true ) 
      {
         return 1;
      }
   }
   return 0;
}


LogicStatus Logic::calculate_status(int pump_value, int water_switch)
{
   
   // If we are on uncertain state, keep old status.
   if ( water_switch != 0 && water_switch != 1 )
      return status;
   
   const LogicStatus states[] = {  LogicStatus::idle, LogicStatus::pump_started, LogicStatus::draining, LogicStatus::pumping };
   return states[ pump_value + (water_switch << 1) ];
}

   
bool Logic::run_logic( const Config_run_table_time* time_now, Device_output* output, int water_level, int water_switch, int manual_switch)
{
   loop_measurement( water_switch );

   if ( poll_timer.check( poll_interval ) == false )
      return false;
   
   poll_timer.reset();
   
   int pump_value = 0;
   if ( manual_switch == 1 )
   {
      pump_value = 1;
      pump_start_check_done = false;
   }
   else
   {
      pump_value = loop_timer( time_now );  
      
      if ( check_timer_safety_ok( pump_value, water_level, water_switch ) == false )
      {
         pump_value = 0;
      }
   }
   
   LogicStatus new_status = calculate_status( pump_value, water_switch);
   
   if ( new_status == status )
      return false;
   int new_status_int = to_integral( new_status );
   LOG_INFO("PLogic new status:  %d", new_status_int ); 
   status = new_status;
   if ( output->get_value() != pump_value )
   {
      pump_timer.reset();
   }
   
   output->set_value( pump_value, true );
   return true;   
}

