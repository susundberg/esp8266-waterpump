#ifndef SUPA_DEVICE_TEMP_H
#define SUPA_DEVICE_TEMP_H

#include "device.h"
#include "stimer.h"

#include "SparkFunHTU21D.h"

class Device_temphum : public Device_input
{
   
public:
   const int poll_interval = 10000;
   // if name starts with 't' then this measures humidity
   Device_temphum( const char* name);
   virtual void loop() override;
   
protected:
   STimer     timer;
   HTU21D     sensor;
};

   


#endif