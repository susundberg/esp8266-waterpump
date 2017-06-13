
/**
This is automatically generated mock file, see header file for more details.
*/


#include "mock_push_thingspeak.h"
DEFINE_FAKE_VOID_FUNC( Push_thingspeak__Push_thingspeak,  Push_thingspeak* );
 Push_thingspeak::Push_thingspeak()  { Push_thingspeak__Push_thingspeak(  this ); }
DEFINE_FAKE_VALUE_FUNC( bool, Push_thingspeak__thingspeak_push,  Push_thingspeak*, const int*, int, int, bool );
bool Push_thingspeak::thingspeak_push(const int* values, int index_start, int values_n, bool force_update)  { return Push_thingspeak__thingspeak_push(  this, values, index_start, values_n, force_update ); }
DEFINE_FAKE_VALUE_FUNC( bool, Push_thingspeak__thingspeak_push_raw,  Push_thingspeak*, const int*, int, int, char* );
bool Push_thingspeak::thingspeak_push_raw(const int* values, int index_start, int values_n, char* buffer)  { return Push_thingspeak__thingspeak_push_raw(  this, values, index_start, values_n, buffer ); }
