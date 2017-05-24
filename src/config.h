
#ifndef SUPA_CONFIG_H
#define SUPA_CONFIG_H


/// Lets provide the 'user side' configuration as struct, so that its easier to change
/// to read those from eeprom or file, if such day comes. 

struct ConfigEmail
{
   const char* server_host;
   int         server_port;
   const char* login;
   const char* password;
   const char* receiver;
};

struct ConfigWlan
{
   const char* sid  ;  
   const char* pass ;
};

struct Config
{
 public:
   ConfigEmail email;
   ConfigWlan  wlan;
   const char* hostname;
   const char* password;
};

extern const Config CONFIG;

/// The hardware side on the other hand may very well be forever fixed.

#define PIN_RTC_SCL 14
#define PIN_RTC_SDA 12
#define PIN_LED     15
#define PIN_PUMP    4
#define PIN_SWITCH  16
#define PIN_TRIGGER 2
#define PIN_ECHO    5
#define PIN_WDETECT 13


#endif
