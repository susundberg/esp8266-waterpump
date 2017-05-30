#include <Arduino.h>
#include "device_wlevel.h"
#include "logger.h"


Device_wlevel::Device_wlevel( const char* name, uint8_t pin_trigger, uint8_t pin_echo ) :
   Device_input(name)
{
   this->pin_echo = pin_echo;
   this->pin_trigger = pin_trigger;
   this->value = 0;
}


void Device_wlevel::setup()
{
   pinMode(pin_trigger, OUTPUT);
   pinMode(pin_echo, INPUT);
   digitalWrite(this->pin_trigger, LOW); 
}


void Device_wlevel::measure_pulse_in()
{
   constexpr const float speed_of_sound_scalar = 0.17175f;
   constexpr const unsigned long max_timeout = max_range_mm / speed_of_sound_scalar;
   
   float duration = pulseIn( this->pin_echo, HIGH, max_timeout );
    
   //Calculate the distance (in mm) based on the speed of sound.
   // c = 343.5 * 1000 / 1000000 = 0.3435 mm/ss
   // and we go there and back -> *.5
   
   float distance = (duration*speed_of_sound_scalar);
    
   if (distance >= this->max_range_mm || distance <= min_range_mm )
   {
      return ; // no update
      nfails += 1;
      
      if ( nfails >= nfail_limit )
      {
         LOG_ERROR("Distance sensor is failing.");
         this->value = 0;
      }
         
   }
   
   nfails = 0;
   float ma_old = this->value;
   const float alpha = 0.5;
   
   
   if ( this->value == 0)
   {  // in the beginning, boost directly to proper level
      ma_old = distance;
   }
   
   float ma_new = alpha * distance + (1-alpha) * ma_old;
   this->value = (int)ma_new;
}

void Device_wlevel::loop()
{
   
   if ( this->timer.check( measure_interval ) == false )
      return;
   
   this->timer.reset();
   
   digitalWrite(this->pin_trigger, LOW); 
   delay( 2 );
   digitalWrite(this->pin_trigger, HIGH);
   delay( 10 );
   digitalWrite(this->pin_trigger, LOW);
   measure_pulse_in();
}
