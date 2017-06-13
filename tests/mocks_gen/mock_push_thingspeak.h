
/**
This is automatically generate mock file (from /home/pauli/DEV/arduino/water-pump/src/push_thingspeak.h). If you wish to edit this, move this to mocks_man directory.
*/
#ifndef _AUTOMOCK_PUSH_THINGSPEAK_H
#define _AUTOMOCK_PUSH_THINGSPEAK_H

#include "fff.h"
#include "push_thingspeak.h"

void Push_thingspeak__Push_thingspeak(  Push_thingspeak* );
DECLARE_FAKE_VOID_FUNC( Push_thingspeak__Push_thingspeak,  Push_thingspeak* );
bool Push_thingspeak__thingspeak_push(  Push_thingspeak*, const int*, int, int, bool );
DECLARE_FAKE_VALUE_FUNC( bool, Push_thingspeak__thingspeak_push,  Push_thingspeak*, const int*, int, int, bool );
bool Push_thingspeak__thingspeak_push_raw(  Push_thingspeak*, const int*, int, int, char* );
DECLARE_FAKE_VALUE_FUNC( bool, Push_thingspeak__thingspeak_push_raw,  Push_thingspeak*, const int*, int, int, char* );

#endif // _AUTOMOCK_PUSH_THINGSPEAK_H
