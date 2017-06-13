/**
*/

#include "mock_logger.h"

Logger LOG;

bool _TEST_log_verbose = false;
Logger::Status _TEST_log_status =  Logger::Status::RUNNING;


Logger::Status _TEST_get_log_status( Logger* log )
{
   return _TEST_log_status;
}

void _TEST_log(Logger* log, Logger::Level level, const __FlashStringHelper* format, va_list* argument_list )  
{
   char output_buffer[256];
   // Make sure logging troubles get caugth by valgrind
   vsnprintf( output_buffer, 256, format, *argument_list );
   
   if (_TEST_log_verbose )
   {
      printf("LOG %d %s\n", (int)level, output_buffer  );
   }
   
   if ( level == Logger::Level::FATAL )
      _TEST_log_status = Logger::Status::ERROR;
}



void _TEST_log_hookup( bool verbose )
{
   Logger__log_fake.custom_fake = _TEST_log;
   Logger__get_status_fake.custom_fake = _TEST_get_log_status;
   _TEST_log_status = Logger::Status::RUNNING;
   _TEST_log_verbose = verbose;
}

DEFINE_FAKE_VOID_FUNC( Logger__log,  Logger*, Logger::Level, const __FlashStringHelper*, va_list* );
void Logger::log(Logger::Level level, const __FlashStringHelper* format, ... )  { va_list ap; va_start(ap, format); Logger__log(  this, level, format, &ap ); va_end(ap); }


DEFINE_FAKE_VOID_FUNC( Logger__Logger,  Logger* );
 Logger::Logger()  { Logger__Logger(  this ); }
DEFINE_FAKE_VOID_FUNC( Logger__setup_serial,  Logger*, const char*, int );
void Logger::setup_serial(const char* hostname, int serial_baudrate)  { Logger__setup_serial(  this, hostname, serial_baudrate ); }
DEFINE_FAKE_VOID_FUNC( Logger__setup_led,  Logger*, int );
void Logger::setup_led(int led_pin_)  { Logger__setup_led(  this, led_pin_ ); }
DEFINE_FAKE_VOID_FUNC( Logger__setup_fatal_hook,  Logger*, LoggerFatalHook );
void Logger::setup_fatal_hook(LoggerFatalHook hook)  { Logger__setup_fatal_hook(  this, hook ); }
DEFINE_FAKE_VALUE_FUNC( Logger::Status, Logger__get_status,  Logger* );
Logger::Status Logger::get_status()  { return Logger__get_status(  this ); }
DEFINE_FAKE_VOID_FUNC( Logger__set_status,  Logger*, Logger::Status );
void Logger::set_status(Logger::Status status)  { Logger__set_status(  this, status ); }
DEFINE_FAKE_VALUE_FUNC( const char*, Logger__get_log_line,  Logger*, int );
const char* Logger::get_log_line(int line_number)  { return Logger__get_log_line(  this, line_number ); }
DEFINE_FAKE_VOID_FUNC( Logger__loop,  Logger* );
void Logger::loop()  { Logger__loop(  this ); }
DEFINE_FAKE_VOID_FUNC( get_uptime, InfoUptime* );