
/**
This is automatically generate mock file (from /home/pauli/DEV/arduino/water-pump/src/logic.h). If you wish to edit this, move this to mocks_man directory.
*/
#ifndef _AUTOMOCK_LOGIC_H
#define _AUTOMOCK_LOGIC_H

#include "fff.h"
#include "logic.h"

void Logic__Logic(  Logic* );
DECLARE_FAKE_VOID_FUNC( Logic__Logic,  Logic* );
bool Logic__run_logic(  Logic*, const Config_run_table_time*, Device_output*, int, int, int );
DECLARE_FAKE_VALUE_FUNC( bool, Logic__run_logic,  Logic*, const Config_run_table_time*, Device_output*, int, int, int );
LogicStatus Logic__get_status(  Logic* );
DECLARE_FAKE_VALUE_FUNC( LogicStatus, Logic__get_status,  Logic* );
bool Logic__get_measurements(  Logic*, int* );
DECLARE_FAKE_VALUE_FUNC( bool, Logic__get_measurements,  Logic*, int* );
void Logic__measure_reset(  Logic* );
DECLARE_FAKE_VOID_FUNC( Logic__measure_reset,  Logic* );
void Logic__loop_measurement(  Logic*, int );
DECLARE_FAKE_VOID_FUNC( Logic__loop_measurement,  Logic*, int );
bool Logic__check_timer_safety_ok(  Logic*, int, int, int );
DECLARE_FAKE_VALUE_FUNC( bool, Logic__check_timer_safety_ok,  Logic*, int, int, int );
int Logic__loop_timer(  Logic*, const Config_run_table_time* );
DECLARE_FAKE_VALUE_FUNC( int, Logic__loop_timer,  Logic*, const Config_run_table_time* );
LogicStatus Logic__calculate_status(  Logic*, int, int );
DECLARE_FAKE_VALUE_FUNC( LogicStatus, Logic__calculate_status,  Logic*, int, int );

#endif // _AUTOMOCK_LOGIC_H
