
/**
This is automatically generated mock file, see header file for more details.
*/


#include "mock_logic.h"
DEFINE_FAKE_VOID_FUNC( Logic__Logic,  Logic* );
 Logic::Logic() : Device("foo") { Logic__Logic(  this ); }
DEFINE_FAKE_VOID_FUNC( Logic__run_logic,  Logic*, const Config_run_table_time*, Device_output*, int, int, int );
void Logic::run_logic(const Config_run_table_time* time, Device_output* output, int water_level, int water_switch, int manual_switch)  { Logic__run_logic(  this, time, output, water_level, water_switch, manual_switch ); }
DEFINE_FAKE_VALUE_FUNC( int, Logic__handle_pump,  Logic*, Device_output*, int, int, int );
int Logic::handle_pump(Device_output* output, int water_level, int water_switch, int period_time)  { return Logic__handle_pump(  this, output, water_level, water_switch, period_time ); }
