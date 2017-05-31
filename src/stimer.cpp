
#include <climits>
#include <Arduino.h>

#include "stimer.h"

STimer::STimer()
{
   this->time_reset = ULONG_MAX/2;
}

void STimer::reset()
{
   this->time_reset = millis();
}

bool STimer::check(unsigned long timeout)
{
   unsigned long target_time  = this->time_reset + timeout;
   unsigned long current_time = millis();
   
   // has the current time overflown:
   if ( current_time < this->time_reset )
   {
      // did the target time overflowed
      if ( this->time_reset < target_time )
      {  // no, it did not -> we are way over.
         return true;
      }
      else
      {  // yes its overflown as well, normal functionality.
         return ( current_time >= target_time );
      }
   }
   else 
   {  // timer has not overflown, how about the target?
      if ( this->time_reset < target_time )
      { // no overflow, here either. Normal business
        return ( current_time >= target_time ); 
      }
      else
      {  // the target is overflown, so must we.
         return false;
      }
   }
}
