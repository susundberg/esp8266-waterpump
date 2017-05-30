#ifndef SUPA_LOGIC_H
#define SUPA_LOGIC_H

#include "device.h"
#include "device_rtc.h"
#include "config.h"
#include "stimer.h"




class Logic : public Device
{
  
   public:
     Logic();
     
     // we could surely have these stored as pointers, but lets minimize the wasted memory here, as we seem to be short on it.
     void run_logic( const Config_run_table_time* time, Device_output* output, int water_level, int water_switch, int manual_switch );

   
   protected:
      int handle_pump(  Device_output* output, int water_level, int water_switch, int period_time );
      
      enum class Pump_status  { stopped, running, error };
      static const int poll_interval = 1000;
      
      STimer timer;
      Pump_status pump_status;

};



#endif