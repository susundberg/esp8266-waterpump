
/**
This is automatically generate mock file (from /home/pauli/DEV/arduino/water-pump/src/logic.h). If you wish to edit this, move this to mocks_man directory.
*/
#ifndef _AUTOMOCK_LOGIC_H
#define _AUTOMOCK_LOGIC_H

#include "fff.h"
#include "logic.h"

void Logic__Logic(  Logic* );
DECLARE_FAKE_VOID_FUNC( Logic__Logic,  Logic* );
void Logic__run_logic(  Logic*, const Config_run_table_time*, Device_output*, int, int, int );
DECLARE_FAKE_VOID_FUNC( Logic__run_logic,  Logic*, const Config_run_table_time*, Device_output*, int, int, int );
int Logic__handle_pump(  Logic*, Device_output*, int, int, int );
DECLARE_FAKE_VALUE_FUNC( int, Logic__handle_pump,  Logic*, Device_output*, int, int, int );

#endif // _AUTOMOCK_LOGIC_H
