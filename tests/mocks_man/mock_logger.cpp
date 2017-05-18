
/**
*/


#include "mock_logger.h"

Logger LOG;
bool _TEST_log_verbose = false;

DEFINE_FAKE_VOID_FUNC( Logger__Logger,  Logger* );
 Logger::Logger()  { Logger__Logger(  this ); }
DEFINE_FAKE_VOID_FUNC( Logger__setup_serial,  Logger*, const char*, int );
void Logger::setup_serial(const char* hostname, int serial_baudrate)  { Logger__setup_serial(  this, hostname, serial_baudrate ); }
DEFINE_FAKE_VOID_FUNC( Logger__setup_led,  Logger*, int );
void Logger::setup_led(int led_pin_)  { Logger__setup_led(  this, led_pin_ ); }
DEFINE_FAKE_VALUE_FUNC( Logger::Status, Logger__get_status,  Logger* );
Logger::Status Logger::get_status()  { return Logger__get_status(  this ); }
DEFINE_FAKE_VOID_FUNC( Logger__set_status,  Logger*, Logger::Status );
void Logger::set_status(Logger::Status status)  { Logger__set_status(  this, status ); }
DEFINE_FAKE_VALUE_FUNC( const char*, Logger__get_log_line,  Logger*, int );
const char* Logger::get_log_line(int line_number)  { return Logger__get_log_line(  this, line_number ); }
DEFINE_FAKE_VOID_FUNC( Logger__loop,  Logger* );
void Logger::loop()  { Logger__loop(  this ); }
DEFINE_FAKE_VOID_FUNC( get_uptime, InfoUptime* );


void Logger::log(Logger::Level level, const __FlashStringHelper* format, ... )  
{

   va_list argument_list;
   va_start ( argument_list, format );       

   
   
   if (_TEST_log_verbose == false )
   {
      char output_buffer[256];
      // Make sure logging troubles get caugth by valgrind
      vsnprintf( output_buffer, 256, format, argument_list );
   }
   else
   {
      printf("LOG %d:", (int)level );
      vprintf( format, argument_list );
      printf("\n");
   }
   
   va_end( argument_list );
   
}

