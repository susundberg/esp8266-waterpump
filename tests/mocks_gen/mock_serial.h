
/**
This is automatically generate mock file (from /home/pauli/DEV/arduino/water-pump/src/serial.h). If you wish to edit this, move this to mocks_man directory.
*/
#ifndef _AUTOMOCK_SERIAL_H
#define _AUTOMOCK_SERIAL_H

#include "fff.h"
#include "serial.h"

DECLARE_FAKE_VOID_FUNC( serial_setup, const char*, int );
DECLARE_FAKE_VALUE_FUNC( char*, serial_receive, int* );
DECLARE_FAKE_VALUE_FUNC( int, serial_receive_number, int, int );
DECLARE_FAKE_VOID_FUNC_VARARG( serial_print, const char*, ... );
DECLARE_FAKE_VOID_FUNC( serial_print_raw, const char*, int, bool );

#endif // _AUTOMOCK_SERIAL_H
