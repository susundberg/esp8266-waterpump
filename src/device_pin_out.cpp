#include "device_pin_out.h"

#include <Arduino.h>

Device_pin_out::Device_pin_out( const char* name, uint8_t pin, uint8_t  value ) :
  Device_output( name )
{
   this->pin = pin;
   this->value = value;
}


void Device_pin_out::setup( ) 
{
   pinMode( this->pin, OUTPUT );
   set_value( this->value );
}


void Device_pin_out::set_value( int value ) 
{
   digitalWrite( this->pin, value );
   this->value = value;
}


