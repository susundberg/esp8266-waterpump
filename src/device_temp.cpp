

#include "device_temp.h"


Device_temphum::Device_temphum( const char* name ) : 
  Device_input( name )
{

}

void Device_temphum::loop()
{
   
   if ( timer.check( poll_interval ) == false )
      return;
   
   timer.reset();
   
   if ( this->name[0] == 't'  )
   {
      value = this->sensor.readTemperature();
   }
   else
   {
      value = this->sensor.readHumidity();
   }
}





