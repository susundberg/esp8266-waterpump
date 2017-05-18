#include <iostream>

#include "WiFiClientSecure.h"

std::list<WiFiClientSecure*> WiFiClientSecure::_test_clients;
std::function<void(WiFiClientSecure*)> WiFiClientSecure::_test_create_hook = NULL;

WiFiClientSecure::WiFiClientSecure()
{
   _test_verbose = false;
   _test_clients.push_front( this );
   
   if (_test_create_hook != NULL )
      _test_create_hook( this );
}

WiFiClientSecure::~WiFiClientSecure()
{
   _test_clients.remove( this );
}


int WiFiClientSecure::available()
{
   int len = 0;
   for (const String& str : _test_input ) 
   {
      len += str.length();
   }
   return len;
}

int WiFiClientSecure::connect(const char* name, uint16_t port)
{
   return 1;
}

int WiFiClientSecure::read(uint8_t *buf, size_t size) 
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



void WiFiClientSecure::setTimeout( int timeout )
{
   
}

void WiFiClientSecure::stop()
{
   
}

void WiFiClientSecure::println( const String& what )
{
   _test_output.push_front( what );
   
   if ( _test_verbose == true )
      std::cout << "Client " << this << " : " << what  << "\n";
}

IPAddress WiFiClientSecure::localIP()
{
   return IPAddress();
}

void WiFiClientSecure::_test_reset()
{
   _test_input_current = String();
   _test_input.clear();
   _test_output.clear();
}


