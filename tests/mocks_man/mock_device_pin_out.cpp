
/**
This is automatically generated mock file, see header file for more details.
*/


#include "mock_device_pin_out.h"
DEFINE_FAKE_VOID_FUNC( Device_pin_out__Device_pin_out,  Device_pin_out*, const char*, uint8_t, bool );
 Device_pin_out::Device_pin_out(const char* name, uint8_t pin, bool invert ) : Device_output( name ) { Device_pin_out__Device_pin_out(  this, name, pin, invert ); }
DEFINE_FAKE_VOID_FUNC( Device_pin_out__setup,  Device_pin_out* );
void Device_pin_out::setup()  { Device_pin_out__setup(  this ); }
DEFINE_FAKE_VOID_FUNC( Device_pin_out__set_value,  Device_pin_out*, int );
void Device_pin_out::set_value(int value, bool verbose)  { Device_pin_out__set_value(  this, value ); }

