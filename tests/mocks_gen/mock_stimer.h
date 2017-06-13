
/**
This is automatically generate mock file (from /home/pauli/DEV/arduino/water-pump/src/stimer.h). If you wish to edit this, move this to mocks_man directory.
*/
#ifndef _AUTOMOCK_STIMER_H
#define _AUTOMOCK_STIMER_H

#include "fff.h"
#include "stimer.h"

void STimer__STimer(  STimer* );
DECLARE_FAKE_VOID_FUNC( STimer__STimer,  STimer* );
void STimer__reset(  STimer* );
DECLARE_FAKE_VOID_FUNC( STimer__reset,  STimer* );
void STimer__reset_with_carry(  STimer*, unsigned long );
DECLARE_FAKE_VOID_FUNC( STimer__reset_with_carry,  STimer*, unsigned long );
bool STimer__check(  STimer*, unsigned long );
DECLARE_FAKE_VALUE_FUNC( bool, STimer__check,  STimer*, unsigned long );
unsigned long STimer__elapsed(  STimer* );
DECLARE_FAKE_VALUE_FUNC( unsigned long, STimer__elapsed,  STimer* );

#endif // _AUTOMOCK_STIMER_H
