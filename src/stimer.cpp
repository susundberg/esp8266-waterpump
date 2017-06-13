

#include <Arduino.h>
#include <limits.h>

#include "stimer.h"

STimer::STimer()
{
   this->reset_ms = ULONG_MAX/2;
}

void STimer::reset()
{
   this->reset_ms = millis();
}

void STimer::reset_with_carry( unsigned long timeout_ms )
{
   // this may overflow, but that is ok!
   this->reset_ms = reset_ms + timeout_ms ;
}

unsigned long STimer::elapsed()
{
   unsigned long current_time = millis();
   if ( current_time >= reset_ms  )
      return ( current_time - reset_ms );
   
   // overflow has occurred.
   return ( ULONG_MAX - reset_ms ) + current_time;
}


bool STimer::check(unsigned long timeout)
{
   unsigned long target_time  = this->reset_ms + timeout;
   unsigned long current_time = millis();
   
   // has the current time overflown:
   if ( current_time < this->reset_ms )
   {
      // did the target time overflowed
      if ( this->reset_ms < target_time )
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
      if ( this->reset_ms < target_time )
      { // no overflow, here either. Normal business
        return ( current_time >= target_time ); 
      }
      else
      {  // the target is overflown, so must we.
         return false;
      }
   }
}
