
#include "logic.h"
#include "logger.h"


Logic::Logic()  : Device("pump_status")
{
   pump_status = Pump_status::stopped;
   timer.reset();
}



static int time_to_secs( const Config_run_table_time* time )
{
   return (time->hour*60 + time->minute)*60 + time->second;
}


int Logic::handle_pump(Device_output* output, int water_level, int water_switch, int period_time )
{
   
   switch ( pump_status )
   {
      case Pump_status::stopped :
         
         // rule 1: we must have water level in high enough (when starting). When pump is running the level will drop.
         if ( water_level <= 0 )
         {
            LOG_ERROR("Water level too low (%d), pump not started.", water_level );
            pump_status = Pump_status::error;
            return 0;
         }
         
         // rule 2: the water switch must be in neutral position (or its stuck)
         if ( water_switch != 0 )
         {
            LOG_ERROR("Water switch still active, pump not started.");
            pump_status = Pump_status::error;
            return 0;
         }
         
         // water level was high enough, pump 
         pump_status  = Pump_status::running;
         return 1;
         
      case Pump_status::running:
         // rule 3: the water switch must detect water after N seconds
         if ( period_time >= CONFIG.pump.threshold_water_up_s )
         {
            if ( water_switch != 1 )
            {
               LOG_ERROR("Water switch not active, even pump started.");
               pump_status = Pump_status::error;
               return 0;
            }
         }
         // all good - either times not up, or all good -> continue pumping
         return 1;
         
      default:
         return 0;
   }
   
}

void Logic::run_logic( const Config_run_table_time* time_now, Device_output* output, int water_level, int water_switch, int manual_switch)
{
   
   value = (int) pump_status;
   
   if ( timer.check( poll_interval ) == false )
      return;
   
   timer.reset();
   
   int time_start_s = time_to_secs( &CONFIG.runtable.active_start );
   int time_stop_s  = time_to_secs( &CONFIG.runtable.active_stop);
   int time_now_s   = time_to_secs( time_now );
   
   // first check if we are driving manual mode
   if ( manual_switch == 1 ) 
   {
      output->set_value(1);
      return;
   }
   
   if ( pump_status == Pump_status::error ) 
   {
      output->set_value(0);
      return;
   }
   
   int pump_running = 0;
   
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
         pump_running = Logic::handle_pump( output, water_level, water_switch, period_rel );
      }
      else
      {
         pump_status = Pump_status::stopped;
      }
   }
   
   output->set_value(pump_running);
   
}

