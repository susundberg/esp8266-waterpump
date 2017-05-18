#ifndef SUPA_WEBSERVER_H
#define SUPA_WEBSERVER_H

#include <ESP8266WebServer.h>

void webserver_loop();
void webserver_setup();
char* webserver_get_buffer();

constexpr const int WEBSERVER_MAX_RESPONSE_SIZE = (4*1024);

extern ESP8266WebServer WEBSERVER;

#endif