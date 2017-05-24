
#ifndef SUPA_DEV_RTC_H
#define SUPA_DEV_RTC_H

#include "device.h"
#include "RTClib.h"
#include "stimer.h"

class Device_rtc : public Device_input
{
   
public:
   Device_rtc( const char* name );
   virtual void loop() override;
   virtual void setup() override;
   void update_time(uint32_t ntp_time);
   
protected:
   RTC_DS1307 rtc;
   STimer     timer;
};


#endif


