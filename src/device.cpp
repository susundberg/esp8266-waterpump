#include <stdio.h>


#include "device.h"

Device::Device( const char* name )
{
   this->name = name;
}

void Device::loop()
{ // nop
}


Device_input::Device_input( const char* name )
  : Device(name)
{
   this->value = 0;
}


int Device_input::jsonify( char* buffer, int buffer_len )
{
   return snprintf( buffer, buffer_len, "\"%s\":{\"value\":%d}", value );
}

int Device_input::get_value() 
{
   return value;
}

