
/**
This is automatically generate mock file (from /home/pauli/DEV/arduino/water-pump/src/logger.h). If you wish to edit this, move this to mocks_man directory.
*/
#ifndef _TEST_CUSTOM_LOGGER_H
#define _TEST_CUSTOM_LOGGER_H


#include "fff.h"
#include "logger.h"

void Logger__Logger(  Logger* );
DECLARE_FAKE_VOID_FUNC( Logger__Logger,  Logger* );
void Logger__setup_serial(  Logger*, const char*, int );
DECLARE_FAKE_VOID_FUNC( Logger__setup_serial,  Logger*, const char*, int );
void Logger__setup_led(  Logger*, int );
DECLARE_FAKE_VOID_FUNC( Logger__setup_led,  Logger*, int );
void Logger__setup_fatal_hook(  Logger*, LoggerFatalHook );
DECLARE_FAKE_VOID_FUNC( Logger__setup_fatal_hook,  Logger*, LoggerFatalHook );
Logger::Status Logger__get_status(  Logger* );
DECLARE_FAKE_VALUE_FUNC( Logger::Status, Logger__get_status,  Logger* );
void Logger__set_status(  Logger*, Logger::Status );
DECLARE_FAKE_VOID_FUNC( Logger__set_status,  Logger*, Logger::Status );
const char* Logger__get_log_line(  Logger*, int );
DECLARE_FAKE_VALUE_FUNC( const char*, Logger__get_log_line,  Logger*, int );
void Logger__loop(  Logger* );
DECLARE_FAKE_VOID_FUNC( Logger__loop,  Logger* );
DECLARE_FAKE_VOID_FUNC( get_uptime, InfoUptime* );

#include <stdarg.h>
void Logger__log(  Logger*, Logger::Level, const __FlashStringHelper*, va_list* ap);
DECLARE_FAKE_VOID_FUNC( Logger__log,  Logger*, Logger::Level, const __FlashStringHelper*, va_list*  );



/// Set this TRUE to get the log prints to stdout.
extern bool _TEST_log_verbose;
extern Logger::Status _TEST_log_status;

void _TEST_log_hookup( bool verbose );


#endif