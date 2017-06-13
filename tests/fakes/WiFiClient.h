#ifndef FAKE_WiFiClient_H
#define FAKE_WiFiClient_H

#include <Arduino.h>
#include "IPAddress.h"
#include "WiFiClient.h"

#include <list>
#include <functional>

class WiFiClient 
{
   public:

      WiFiClient();
     ~WiFiClient();
     
     int available() ;
     int connect(const char* name, uint16_t port) ;
     int read(uint8_t *buf, size_t size) ;
     void write( const uint8_t *buf, size_t size ) ;
     
     void setTimeout( int timeout );
     void stop();
     void println( const String& what );
     
     IPAddress localIP();
     
     typedef std::list<std::string> Buffer; 
     void _test_reset();
     Buffer _test_output;
     Buffer _test_input;
     std::string _test_input_current;
     static bool _test_verbose;
     
     // class variable where all the clients go
     static std::list<WiFiClient*> _test_clients;
     static std::function<void(WiFiClient*)> _test_create_hook;
};

#endif