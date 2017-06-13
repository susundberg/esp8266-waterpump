#include <iostream>

#include "WiFiClient.h"

std::list<WiFiClient*> WiFiClient::_test_clients;
std::function<void(WiFiClient*)> WiFiClient::_test_create_hook = NULL;
bool WiFiClient::_test_verbose = false;

WiFiClient::WiFiClient()
{
   _test_clients.push_front( this );
   
   if (_test_create_hook != NULL )
      _test_create_hook( this );
}

WiFiClient::~WiFiClient()
{
   _test_clients.remove( this );
}


int WiFiClient::available()
{
   int len = 0;
   for (const String& str : _test_input ) 
   {
      len += str.length();
   }
   return len;
}

int WiFiClient::connect(const char* name, uint16_t port)
{
   return 1;
}

int WiFiClient::read(uint8_t *buf, size_t size) 
{
   
   if ( _test_input_current.length() == 0 )
   {
      if ( _test_input.empty() == true )
         return 0;
      
      _test_input_current = _test_input.back();
      _test_input.pop_back();
   }
   
      
   int to_copy_len  = std::min( size, _test_input_current.length() );
   String to_copy =_test_input_current.substr( 0, to_copy_len );
   _test_input_current = _test_input_current.substr( to_copy_len, std::string::npos );
   
   memcpy( buf, to_copy.c_str(), to_copy_len );
   return to_copy_len;
}



void WiFiClient::setTimeout( int timeout )
{
   
}

void WiFiClient::stop()
{
   
}

void WiFiClient::println( const String& what )
{
   _test_output.push_front( what );
   
   if ( _test_verbose == true )
      std::cout << "Client " << this << " : " << what  << "\n";
}

void WiFiClient::write( const uint8_t* buffer, size_t buffer_n )
{
   std::string str;
   str.assign( (const char*)buffer, buffer_n );
   println( str );
}

IPAddress WiFiClient::localIP()
{
   return IPAddress();
}

void WiFiClient::_test_reset()
{
   _test_input_current = String();
   _test_input.clear();
   _test_output.clear();
}


