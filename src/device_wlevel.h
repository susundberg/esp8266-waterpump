

#ifndef SUPA_DISTANCE_H
#define SUPA_DISTANCE_H

#include <stdint.h>

#include "device.h"
#include "stimer.h"



/** Device to measure water level. The value will be mm from baselevel, value 0 == danger low level.
 */

class Device_wlevel : public Device_input
{
public:
   static const int nfail_limit        = 16; // how many fails before reporting
   static const int measure_interval   = 10000; 
   
   Device_wlevel( const char* name, uint8_t pin_trigger, uint8_t pin_echo );
   virtual void loop() override;
   virtual void setup() override;
protected:
   
   void measure_pulse_in();
   
   STimer timer;
   uint8_t pin_trigger;
   uint8_t pin_echo;
   uint8_t nfails;
};



#endif