
/**
This is automatically generate mock file (from /home/pauli/DEV/arduino/water-pump/src/device.h). If you wish to edit this, move this to mocks_man directory.
*/
#ifndef _AUTOMOCK_DEVICE_H
#define _AUTOMOCK_DEVICE_H

#include "fff.h"
#include "device.h"

void Device__Device(  Device*, const char* );
DECLARE_FAKE_VOID_FUNC( Device__Device,  Device*, const char* );
int Device__jsonify(  Device*, char*, int );
DECLARE_FAKE_VALUE_FUNC( int, Device__jsonify,  Device*, char*, int );
void Device__loop(  Device* );
DECLARE_FAKE_VOID_FUNC( Device__loop,  Device* );
void Device__setup(  Device* );
DECLARE_FAKE_VOID_FUNC( Device__setup,  Device* );
void Device_input__Device_input(  Device_input*, const char* );
DECLARE_FAKE_VOID_FUNC( Device_input__Device_input,  Device_input*, const char* );
int Device_input__get_value(  Device_input* );
DECLARE_FAKE_VALUE_FUNC( int, Device_input__get_value,  Device_input* );
void Device_output__Device_output(  Device_output*, const char* );
DECLARE_FAKE_VOID_FUNC( Device_output__Device_output,  Device_output*, const char* );
void Device_output__set_value(  Device_output*, int );
DECLARE_FAKE_VOID_FUNC( Device_output__set_value,  Device_output*, int );

#endif // _AUTOMOCK_DEVICE_H
