#ifndef SUPA_LOGIC_H
#define SUPA_LOGIC_H

#include "device.h"
#include "device_rtc.h"
#include "config.h"
#include "stimer.h"

/// Placed in public section due test-framework limitations
/// Idle = nothing, all clear = pump=0, detect=0
/// started  = pump=1, detect=0
/// pumping  = pump=1, detect=1
/// draining = pump=0, detect=1
enum class LogicStatus  { idle, pump_started, draining, pumping,  }; 

/** The main logic, its not really a device, but lets use it as such, since that we we can easily get a value out of it */
class Logic 
{
  
   public:
     
     
     Logic();
     
     /// we could surely have these stored as pointers, but lets minimize the wasted memory here, as we seem to be short on it.
     /// @returns true if state has changed
     bool run_logic( const Config_run_table_time* time, Device_output* output, int water_level, int water_switch, int manual_switch );

     LogicStatus get_status(); 
     /// @returns true if measurements updated and both valid
     bool get_measurements( int* delays );
     
   protected:
      void measure_reset();
      void loop_measurement( int water_switch );
      bool check_timer_safety_ok(int pump, int water_level, int water_switch );
      int  loop_timer( const Config_run_table_time* time_now );
      LogicStatus calculate_status(int pump_value, int water_switch);
      
      static const int poll_interval = 100;
      
      STimer       pump_timer;
      STimer       poll_timer;
      
      unsigned int measure_delay_p1_d1;
      unsigned int measure_delay_p0_d0;
      
      LogicStatus  status;
      bool pump_start_check_done;

};



#endif