#include "device_pin_out.h"

#include <Arduino.h>

Device_pin_out::Device_pin_out( const char* name, uint8_t pin, bool invert ) :
  Device_output( name )
{
   this->pin = pin;
   this->invert = invert;
}


void Device_pin_out::setup( ) 
{
   pinMode( this->pin, OUTPUT );
   set_value( this->value );
}


void Device_pin_out::set_value( int value ) 
{
   bool write_value = (invert == false) ? (value != 0) : (value == 0);
   digitalWrite( this->pin, write_value );
   this->value = value;
}


