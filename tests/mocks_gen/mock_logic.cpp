
/**
This is automatically generated mock file, see header file for more details.
*/


#include "mock_logic.h"
DEFINE_FAKE_VOID_FUNC( Logic__Logic,  Logic* );
 Logic::Logic()  { Logic__Logic(  this ); }
DEFINE_FAKE_VALUE_FUNC( bool, Logic__run_logic,  Logic*, const Config_run_table_time*, Device_output*, int, int, int );
bool Logic::run_logic(const Config_run_table_time* time, Device_output* output, int water_level, int water_switch, int manual_switch)  { return Logic__run_logic(  this, time, output, water_level, water_switch, manual_switch ); }
DEFINE_FAKE_VALUE_FUNC( LogicStatus, Logic__get_status,  Logic* );
LogicStatus Logic::get_status()  { return Logic__get_status(  this ); }
DEFINE_FAKE_VALUE_FUNC( bool, Logic__get_measurements,  Logic*, int* );
bool Logic::get_measurements(int* delays)  { return Logic__get_measurements(  this, delays ); }
DEFINE_FAKE_VOID_FUNC( Logic__measure_reset,  Logic* );
void Logic::measure_reset()  { Logic__measure_reset(  this ); }
DEFINE_FAKE_VOID_FUNC( Logic__loop_measurement,  Logic*, int );
void Logic::loop_measurement(int water_switch)  { Logic__loop_measurement(  this, water_switch ); }
DEFINE_FAKE_VALUE_FUNC( bool, Logic__check_timer_safety_ok,  Logic*, int, int, int );
bool Logic::check_timer_safety_ok(int pump, int water_level, int water_switch)  { return Logic__check_timer_safety_ok(  this, pump, water_level, water_switch ); }
DEFINE_FAKE_VALUE_FUNC( int, Logic__loop_timer,  Logic*, const Config_run_table_time* );
int Logic::loop_timer(const Config_run_table_time* time_now)  { return Logic__loop_timer(  this, time_now ); }
DEFINE_FAKE_VALUE_FUNC( LogicStatus, Logic__calculate_status,  Logic*, int, int );
LogicStatus Logic::calculate_status(int pump_value, int water_switch)  { return Logic__calculate_status(  this, pump_value, water_switch ); }
