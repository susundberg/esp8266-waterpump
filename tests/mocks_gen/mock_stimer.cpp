
/**
This is automatically generated mock file, see header file for more details.
*/


#include "mock_stimer.h"
DEFINE_FAKE_VOID_FUNC( STimer__STimer,  STimer* );
 STimer::STimer()  { STimer__STimer(  this ); }
DEFINE_FAKE_VOID_FUNC( STimer__reset,  STimer* );
void STimer::reset()  { STimer__reset(  this ); }
DEFINE_FAKE_VALUE_FUNC( bool, STimer__check,  STimer*, unsigned long );
bool STimer::check(unsigned long timeout)  { return STimer__check(  this, timeout ); }
