#include "device_pin_out.h"

#include "logger.h"
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


void Device_pin_out::set_value( int value, bool verbose ) 
{
   bool write_value = (invert == false) ? (value != 0) : (value == 0);
   digitalWrite( this->pin, write_value );
   
   if ( this->value != value && verbose )
   {
      LOG_INFO("Dev '%s': %d", this->name, value );
   }
   this->value = value;
}


