#include "device_pin_in.h"

#include <Arduino.h>


Device_pin_in::Device_pin_in( const char* name, uint8_t pin, uint8_t filter_len ):
Device_input( name )
{
   this->pin = pin;
   this->filter_len = filter_len;
   this->filter_sum = 0;
   this->value = -1;
}

void Device_pin_in::setup()
{
   pinMode( this->pin, INPUT_PULLUP );
}

void Device_pin_in::loop()
{
   if ( timer.check( poll_interval ) == false )
      return;
   
   timer.reset();
   
   int val = digitalRead( this->pin );
   
   this->filter_sum += (val*2 - 1);
   
   if ( this->filter_sum > filter_len / 2 )
   {
      this->value = 1;
      if ( this->filter_sum > filter_len )
      {
         this->filter_sum = filter_len ;
      }
   }
   else if ( this->filter_sum < -(filter_len/2) )
   {
      this->value = 0;
      if ( this->filter_sum < -filter_len )
      {
         this->filter_sum = -filter_len ;
      }
   }
   else
   {
      this->value = -1;
   }
}
