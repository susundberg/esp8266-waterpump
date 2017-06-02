#ifndef SUPA_EMAIL_SEND
#define SUPA_EMAIL_SEND

#define EMAIL_SEND_MAX_SIZE   4096
#define EMAIL_SEND_TIMEOUT_MS 2000
#define EMAIL_SEND_MAX_MAILS  3    // How many emails may the device send during single boot. Now its fixed but we could have quota updated
#include "config.h"


/// @returns true if sending has succeeded.
bool email_send( const Config_email* settings, const char* receiver, const char* subject, const char* message );



#endif

