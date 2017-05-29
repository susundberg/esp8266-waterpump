
/**
This is automatically generated mock file, see header file for more details.
*/


#include "mock_device_pin_in.h"
DEFINE_FAKE_VOID_FUNC( Device_pin_in__Device_pin_in,  Device_pin_in*, const char*, uint8_t, uint8_t );
 Device_pin_in::Device_pin_in(const char* name, uint8_t pin, uint8_t filter_len) : Device_input(name) { Device_pin_in__Device_pin_in(  this, name, pin, filter_len ); }
DEFINE_FAKE_VOID_FUNC( Device_pin_in__setup,  Device_pin_in* );
void Device_pin_in::setup()  { Device_pin_in__setup(  this ); }
DEFINE_FAKE_VOID_FUNC( Device_pin_in__loop,  Device_pin_in* );
void Device_pin_in::loop()  { Device_pin_in__loop(  this ); }
