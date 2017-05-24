#include <Arduino.h>
#include "device_wlevel.h"
#include "logger.h"


Device_wlevel::Device_wlevel( const char* name, uint8_t pin_trigger, uint8_t pin_echo ) :
   Device_input(name)
{
   this->pin_echo = pin_echo;
   this->pin_trigger = pin_trigger;
   this->value = 0;
   this->state_timeout = 1000;
   this->state = 0;
}


void Device_wlevel::setup()
{
   pinMode(pin_trigger, OUTPUT);
   pinMode(pin_echo, INPUT);
   digitalWrite(this->pin_trigger, LOW); 
}


void Device_wlevel::measure_pulse_in()
{
   float duration = pulseIn( this->pin_echo, HIGH);
    
   //Calculate the distance (in cm) based on the speed of sound.
   // c = 343.5 * 1000 / 1000000 = 0.3435 mm/ss
   // and we go there and back -> *.5
   
   float distance = (duration*0.17175f);
    
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
   
   if ( this->timer.check( this->state_timeout ) == false )
      return;
   
   this->timer.reset();
   
   switch( this->state )
   {
      case 1:
         digitalWrite(this->pin_trigger, LOW); 
         this->state_timeout = 2;
         break;
         
      case 2:
         digitalWrite(this->pin_trigger, HIGH);
         this->state_timeout = 10;
         break;
      
      case 3:
         digitalWrite(this->pin_trigger, LOW);
         measure_pulse_in();
         this->state_timeout = 1000;
         break;
         
      default:
         this->state = 0; // it will be increment outside of the loop -> 1 on next round
         this->state_timeout = 0;
         break;
   }
   
   this->state = this->state + 1;

}
