#ifndef SUPA_EMAIL_SEND
#define SUPA_EMAIL_SEND

#define EMAIL_SEND_MAX_SIZE   4096
#define EMAIL_SEND_TIMEOUT_MS 2000

#include "config.h"


/// @returns true if sending has succeeded.
bool email_send( const Config_email* settings, const char* receiver, const char* subject, const char* message );



#endif

