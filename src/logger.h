#ifndef SUPA_LOGGER_H
#define SUPA_LOGGER_H


#include <ESP8266WiFi.h>

#include "stimer.h"

class Logger
{
   public:
      
     enum class Status { BOOTING, RUNNING, ERROR };
     enum class Level { DEBUG, INFO, WARNING, ERROR };
     
     static const int max_lines = 16;
     static const int max_line_len = 80;

     Logger();  // plain initialization, sets status to booting
     
     void setup_serial( const char* hostname, int serial_baudrate=115200 );
     void setup_led( int led_pin_ );
     
     Logger::Status get_status();
     void set_status( Logger::Status status );

     void log( Logger::Level level, const __FlashStringHelper* format, ... );
     
     /// @returns pointer to given line or NULL if not that many lines.
     const char* get_log_line( int line_number );
     
     void loop(); // blink led if defined
     
  private:
     STimer led_timer;
     int led_pin;
     bool led_state;
     int line_loop;
     int serial_baudrate;
     Logger::Status status;
     char buffer[ max_lines*max_line_len ];
     char format[ max_line_len ];
     
};

struct InfoUptime 
{
   unsigned int hours;
   unsigned int minutes;
   unsigned int seconds;
};

void get_uptime( InfoUptime* uptime );

extern Logger LOG;

#define LOG_INFO( format, ... ) LOG.log( Logger::Level::INFO, F(format) , ## __VA_ARGS__ )
#define LOG_WARN( format, ... ) LOG.log( Logger::Level::WARNING, F(format), ## __VA_ARGS__ )
#define LOG_ERROR( format, ... ) LOG.log( Logger::Level::ERROR, F(format), ## __VA_ARGS__ )

#endif