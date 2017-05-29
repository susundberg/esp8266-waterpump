
/**
This is automatically generate mock file (from /home/pauli/DEV/arduino/water-pump/src/platform.h). If you wish to edit this, move this to mocks_man directory.
*/
#ifndef _AUTOMOCK_PLATFORM_H
#define _AUTOMOCK_PLATFORM_H

#include "fff.h"
#include "platform.h"

void Platform_ESP8266__setup(  Platform_ESP8266* );
DECLARE_FAKE_VOID_FUNC( Platform_ESP8266__setup,  Platform_ESP8266* );
void Platform_ESP8266__loop(  Platform_ESP8266* );
DECLARE_FAKE_VOID_FUNC( Platform_ESP8266__loop,  Platform_ESP8266* );
bool Platform_ESP8266__connected(  Platform_ESP8266* );
DECLARE_FAKE_VALUE_FUNC( bool, Platform_ESP8266__connected,  Platform_ESP8266* );
unsigned int Platform_ESP8266__get_free_heap(  Platform_ESP8266* );
DECLARE_FAKE_VALUE_FUNC( unsigned int, Platform_ESP8266__get_free_heap,  Platform_ESP8266* );
void Platform_ESP8266__setup_wifi(  Platform_ESP8266* );
DECLARE_FAKE_VOID_FUNC( Platform_ESP8266__setup_wifi,  Platform_ESP8266* );
void Platform_ESP8266__setup_i2c(  Platform_ESP8266* );
DECLARE_FAKE_VOID_FUNC( Platform_ESP8266__setup_i2c,  Platform_ESP8266* );
void Platform_ESP8266__setup_ota(  Platform_ESP8266* );
DECLARE_FAKE_VOID_FUNC( Platform_ESP8266__setup_ota,  Platform_ESP8266* );
void Platform_ESP8266__setup_fs(  Platform_ESP8266* );
DECLARE_FAKE_VOID_FUNC( Platform_ESP8266__setup_fs,  Platform_ESP8266* );
void Platform_ESP8266__loop_wifi(  Platform_ESP8266* );
DECLARE_FAKE_VOID_FUNC( Platform_ESP8266__loop_wifi,  Platform_ESP8266* );
void Platform_ESP8266__loop_ota(  Platform_ESP8266* );
DECLARE_FAKE_VOID_FUNC( Platform_ESP8266__loop_ota,  Platform_ESP8266* );

#endif // _AUTOMOCK_PLATFORM_H
