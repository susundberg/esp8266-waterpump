
#include "ESP8266WebServer.h"

ESP8266WebServer::ESP8266WebServer(int port)
{
}
  
ESP8266WebServer::~ESP8266WebServer()
{
}

void ESP8266WebServer::begin()
{
     this->_test_started = true;
}
  
void ESP8266WebServer::handleClient()
{
     
}

void ESP8266WebServer::on(const char* uri, THandlerFunction handler)
{
   this->handlers.push_back( THandlerPair( std::string( uri ), handler ) );
}

void ESP8266WebServer::serveStatic(const char* uri, fs::FS& fs, const char* path, const char* cache_header )
{
   
}

void ESP8266WebServer::_test_clear()
{
   this->_test_sent_code = -1;
   this->_test_sent_content = std::string();
   this->_test_sent_header = std::string();
   this->handlers.clear();
}

bool ESP8266WebServer::_test_serve( const char* url )
{
   std::string to_match(url);
   for ( auto&& handler: this->handlers )
   {
      if ( handler.first == url )
      {
         handler.second();
         return true;
      }
   }
   return false;
}


void ESP8266WebServer::onNotFound(THandlerFunction fn)
{
  this->on("__404__", fn);
}

void ESP8266WebServer::send(int code, const char* content_type , const String& content )
{
   this->_test_sent_code    = code;
   this->_test_sent_content = std::string( content );
   this->_test_sent_header  = std::string( content_type );   
}
