#include <stdio.h>

#include "logger.h"
#include "device.h"

Device::Device( const char* name )
{
   this->name = name;
   this->value = 0;
}

void Device::loop()
{ // nop
}

void Device::setup()
{  // nop
}


Device_input::Device_input( const char* name )
  : Device(name)
{
}


Device_output::Device_output( const char* name )
  : Device(name)
{
}

void Device_input::update_value(int new_value)
{
   if ( value == new_value )
      return;
   
   this->value = new_value;
   LOG_INFO("Dev %s: value now %d", this->name, value );
}


int Device::jsonify( char* buffer, int buffer_len )
{
   int len = snprintf( buffer, buffer_len, "{\"name\":\"%s\",\"value\":%d}", name, value );
   if (len >= buffer_len)
   {
      LOG_ERROR("Too long JSON: '%s'", name );
      return 0;
   }
   return len;
}

int Device::get_value() const
{
   return value;
}


