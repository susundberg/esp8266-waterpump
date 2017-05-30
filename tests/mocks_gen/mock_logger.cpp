
/**
This is automatically generated mock file, see header file for more details.
*/


#include "mock_logger.h"
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
DEFINE_FAKE_VOID_FUNC_VARARG( Logger__log,  Logger*, Logger::Level, const __FlashStringHelper*, ... );
void Logger::log(Logger::Level level, const __FlashStringHelper* format, ... )  { Logger__log(  this, level, format ); }
DEFINE_FAKE_VALUE_FUNC( const char*, Logger__get_log_line,  Logger*, int );
const char* Logger::get_log_line(int line_number)  { return Logger__get_log_line(  this, line_number ); }
DEFINE_FAKE_VOID_FUNC( Logger__loop,  Logger* );
void Logger::loop()  { Logger__loop(  this ); }
DEFINE_FAKE_VOID_FUNC( get_uptime, InfoUptime* );
