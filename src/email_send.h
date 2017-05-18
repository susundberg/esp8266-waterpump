#ifndef SUPA_EMAIL_SEND
#define SUPA_EMAIL_SEND

#define EMAIL_SEND_MAX_SIZE   4096
#define EMAIL_SEND_TIMEOUT_MS 2000

struct EmailSettings
{
   const char* login; // example: veikko.vene@gmail.com
   const char* password; // app specific password, 16chars
   const char* sender; // in google same as login
   const char* server_host; // smtp.google.com
   int   server_port ;   // server port, 465
} ;

/// @returns true if sending has succeeded.
bool email_send( const EmailSettings* settings, const char* receiver, const char* subject, const char* message );



#endif

