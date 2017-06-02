
/**
This is automatically generate mock file (from /home/pauli/DEV/arduino/water-pump/src/device_pin_in.h). If you wish to edit this, move this to mocks_man directory.
*/
#ifndef _AUTOMOCK_DEVICE_PIN_IN_H
#define _AUTOMOCK_DEVICE_PIN_IN_H

#include "fff.h"
#include "device_pin_in.h"

void Device_pin_in__Device_pin_in(  Device_pin_in*, const char*, uint8_t, uint8_t, bool );
DECLARE_FAKE_VOID_FUNC( Device_pin_in__Device_pin_in,  Device_pin_in*, const char*, uint8_t, uint8_t, bool );
void Device_pin_in__setup(  Device_pin_in* );
DECLARE_FAKE_VOID_FUNC( Device_pin_in__setup,  Device_pin_in* );
void Device_pin_in__loop(  Device_pin_in* );
DECLARE_FAKE_VOID_FUNC( Device_pin_in__loop,  Device_pin_in* );

class Device_pin_in_FAKE : public Device_pin_in
{
  public:
    Device_pin_in_FAKE( int number );
};

#endif // _AUTOMOCK_DEVICE_PIN_IN_H
