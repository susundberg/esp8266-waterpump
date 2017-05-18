
/**
This is automatically generate mock file (from /home/pauli/DEV/arduino/water-pump/src/ntp_client.h). If you wish to edit this, move this to mocks_man directory.
*/
#ifndef _AUTOMOCK_NTP_CLIENT_H
#define _AUTOMOCK_NTP_CLIENT_H

#include "fff.h"
#include "ntp_client.h"

DECLARE_FAKE_VOID_FUNC( ntp_client_loop );
DECLARE_FAKE_VOID_FUNC( ntp_client_setup );
DECLARE_FAKE_VALUE_FUNC( unsigned long, ntp_client_gettime );
DECLARE_FAKE_VALUE_FUNC( bool, ntp_client_valid );

#endif // _AUTOMOCK_NTP_CLIENT_H
