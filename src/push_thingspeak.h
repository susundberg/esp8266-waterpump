#ifndef SUPA_THINGSPEAK_H
#define SUPA_THINGSPEAK_H

#include "stimer.h"
#include "device.h"




class Push_thingspeak
{
   public:
      Push_thingspeak();
      
      /// @param values must be array of all fields in proper order. returns true on success (pushes every now and then, not every call)
      /// @param values_n - how many values to be pushed
      /// @param force_update - do we force update
      bool thingspeak_push( const Device_input** values, int values_n, bool force_update );   
      
      static const int push_interval_s = 15*60;
      static const int buffer_size     = 4096;
      
   protected:
      STimer timer;
      bool thingspeak_push_raw( const Device_input** values, int values_n, char* buffer );   
};

#endif