
/**
This is automatically generate mock file (from /home/pauli/DEV/arduino/water-pump/src/logger.h). If you wish to edit this, move this to mocks_man directory.
*/
#ifndef _AUTOMOCK_LOGGER_H
#define _AUTOMOCK_LOGGER_H

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
void Logger__log(  Logger*, Logger::Level, const __FlashStringHelper*, ... );
DECLARE_FAKE_VOID_FUNC_VARARG( Logger__log,  Logger*, Logger::Level, const __FlashStringHelper*, ... );
const char* Logger__get_log_line(  Logger*, int );
DECLARE_FAKE_VALUE_FUNC( const char*, Logger__get_log_line,  Logger*, int );
void Logger__loop(  Logger* );
DECLARE_FAKE_VOID_FUNC( Logger__loop,  Logger* );
DECLARE_FAKE_VOID_FUNC( get_uptime, InfoUptime* );

#endif // _AUTOMOCK_LOGGER_H
