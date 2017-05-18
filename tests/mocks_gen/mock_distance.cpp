
/**
This is automatically generated mock file, see header file for more details.
*/


#include "mock_distance.h"
DEFINE_FAKE_VOID_FUNC( Distance__setup,  Distance*, int, int );
void Distance::setup(int pin_trigger, int pin_echo)  { Distance__setup(  this, pin_trigger, pin_echo ); }
DEFINE_FAKE_VALUE_FUNC( int, Distance__get_distance,  Distance* );
int Distance::get_distance()  { return Distance__get_distance(  this ); }
