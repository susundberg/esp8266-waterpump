
/**
This is automatically generated mock file, see header file for more details.
*/


#include "mock_device.h"
DEFINE_FAKE_VOID_FUNC( Device__Device,  Device*, const char* );
 Device::Device(const char* name)  { Device__Device(  this, name ); }
DEFINE_FAKE_VALUE_FUNC( int, Device__jsonify,  Device*, char*, int );
int Device::jsonify(char* buffer, int buffer_len)  { return Device__jsonify(  this, buffer, buffer_len ); }
DEFINE_FAKE_VOID_FUNC( Device__loop,  Device* );
void Device::loop()  { Device__loop(  this ); }
DEFINE_FAKE_VOID_FUNC( Device__setup,  Device* );
void Device::setup()  { Device__setup(  this ); }
DEFINE_FAKE_VOID_FUNC( Device_input__Device_input,  Device_input*, const char* );
 Device_input::Device_input(const char* name)  : Device(name) { Device_input__Device_input(  this, name ); }

int Device::get_value() const  { return this->value; }
DEFINE_FAKE_VOID_FUNC( Device_output__Device_output,  Device_output*, const char* );
 Device_output::Device_output(const char* name) : Device(name)  { Device_output__Device_output(  this, name ); }
DEFINE_FAKE_VOID_FUNC( Device_output__set_value,  Device_output*, int );
void Device_output::set_value(int value, bool verbose)  { Device_output__set_value(  this, value ); }
