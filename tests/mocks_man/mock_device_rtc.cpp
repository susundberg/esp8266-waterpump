
/**
This is automatically generated mock file, see header file for more details.
*/


#include "mock_device_rtc.h"
DEFINE_FAKE_VOID_FUNC( Device_rtc__Device_rtc,  Device_rtc*, const char* );
 Device_rtc::Device_rtc(const char* name) : Device_input( name )  { Device_rtc__Device_rtc(  this, name ); }
DEFINE_FAKE_VOID_FUNC( Device_rtc__loop,  Device_rtc* );
void Device_rtc::loop()  { Device_rtc__loop(  this ); }
DEFINE_FAKE_VOID_FUNC( Device_rtc__setup,  Device_rtc* );
void Device_rtc::setup()  { Device_rtc__setup(  this ); }
DEFINE_FAKE_VOID_FUNC( Device_rtc__update_time,  Device_rtc*, uint32_t );
void Device_rtc::update_time(uint32_t ntp_time)  { Device_rtc__update_time(  this, ntp_time ); }

DEFINE_FAKE_VOID_FUNC( Device_rtc__time_of_day,  Device_rtc*, Config_run_table_time* );
void Device_rtc::time_of_day( Config_run_table_time* now )  { Device_rtc__time_of_day(  this, now ); }