#ifndef FAKE_WiFiClientSecure_H
#define FAKE_WiFiClientSecure_H

#include <Arduino.h>
#include "IPAddress.h"


#include <list>
#include <functional>

class WiFiClientSecure
{
   public:
     
     
     
     WiFiClientSecure();
     ~WiFiClientSecure();
     
     int available() ;
     int connect(const char* name, uint16_t port) ;
     int read(uint8_t *buf, size_t size) ;
     void setTimeout( int timeout );
     void stop();
     void println( const String& what );
     
     IPAddress localIP();
     
     typedef std::list<std::string> Buffer; 
     void _test_reset();
     Buffer _test_output;
     Buffer _test_input;
     std::string _test_input_current;
     bool _test_verbose;
     
     // class variable where all the clients go
     static std::list<WiFiClientSecure*> _test_clients;
     static std::function<void(WiFiClientSecure*)> _test_create_hook;
};

#endif