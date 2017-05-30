
#ifndef SUPA_DEV_RTC_H
#define SUPA_DEV_RTC_H

#include <stdint.h>

#include "device.h"
#include "RTClib.h"
#include "stimer.h"
#include "config.h"


class Device_rtc : public Device_input
{
   
public:
   Device_rtc( const char* name );
   virtual void loop() override;
   virtual void setup() override;
   void update_time(uint32_t ntp_time);
   void time_of_day( Config_run_table_time* time );
   
protected:
   RTC_DS1307 rtc;
   STimer     timer;
};


#endif


