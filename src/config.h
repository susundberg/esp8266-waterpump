
#ifndef SUPA_CONFIG_H
#define SUPA_CONFIG_H

#include <stdint.h>

/// Lets provide the 'user side' configuration as struct, so that its easier to change
/// to read those from eeprom or file, if such day comes. 

struct Config_email
{
   const char* server_host;
   int         server_port;
   const char* login;
   const char* password;
   const char* receiver;
};

struct Config_wlan
{
   const char* sid  ;  
   const char* pass ;
};


struct Config_run_table_time
{
   uint8_t hour;
   uint8_t minute;
   uint8_t second;
};

struct Config_run_table
{
   Config_run_table_time active_start; // at what hour does the active
   Config_run_table_time active_stop;
   Config_run_table_time period_on;
   Config_run_table_time period_off;
};

struct Config_pump
{
   uint16_t low_level_height_mm      ;  // what is the min distance we are still allowed to start pumping  
   uint8_t threshold_water_up_s ;
};

struct Config
{
   Config_email email;
   Config_wlan  wlan;
   Config_run_table runtable;
   Config_pump pump;
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
