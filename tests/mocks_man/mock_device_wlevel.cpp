
/**
This is automatically generated mock file, see header file for more details.
*/


#include "mock_device_wlevel.h"
DEFINE_FAKE_VOID_FUNC( Device_wlevel__Device_wlevel,  Device_wlevel*, const char*, uint8_t, uint8_t );
 Device_wlevel::Device_wlevel(const char* name, uint8_t pin_trigger, uint8_t pin_echo)  : Device_input( name ) { Device_wlevel__Device_wlevel(  this, name, pin_trigger, pin_echo ); }
DEFINE_FAKE_VOID_FUNC( Device_wlevel__loop,  Device_wlevel* );
void Device_wlevel::loop()  { Device_wlevel__loop(  this ); }
DEFINE_FAKE_VOID_FUNC( Device_wlevel__setup,  Device_wlevel* );
void Device_wlevel::setup()  { Device_wlevel__setup(  this ); }
DEFINE_FAKE_VOID_FUNC( Device_wlevel__measure_pulse_in,  Device_wlevel* );
void Device_wlevel::measure_pulse_in()  { Device_wlevel__measure_pulse_in(  this ); }
