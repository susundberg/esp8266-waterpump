
/**
This is automatically generated mock file, see header file for more details.
*/


#include "mock_serial.h"
DEFINE_FAKE_VOID_FUNC( serial_setup, const char*, int );
DEFINE_FAKE_VALUE_FUNC( char*, serial_receive, int* );
DEFINE_FAKE_VALUE_FUNC( int, serial_receive_number, int, int );
DEFINE_FAKE_VOID_FUNC_VARARG( serial_print, const char*, ... );
DEFINE_FAKE_VOID_FUNC( serial_print_raw, const char*, int, bool );
