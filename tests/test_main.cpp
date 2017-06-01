#include <iostream>


#include "catch.hpp"
#include "Arduino.h"

#include "config.h"
#include "mock_logger.h"
#include "mock_device.h"
#include "mock_webserver.h"
#include "mock_ntp_client.h"
#include "mock_email_send.h"
#include "mock_platform.h"

extern void setup();
extern void loop();

#include "platform.h"
#include "webserver.h"

Platform_ESP8266 PLATFORM;
ESP8266WebServer WEBSERVER(80);


static int dev_jsonify( Device* dev, char* buffer, int len )
{
   return snprintf( buffer, len, "{\"name\":\"%lx\",\"value\":%d}", (unsigned long)dev, (int)(((uintptr_t)dev)%10) );
}

bool check_email( const Config_email* settings, const char* receiver, const char* subject, const char* message )
{
   std::cout << "EMAIL SUBJECT: " << subject << "\n";
   std::cout << message << "\n";
   return true;
}

char* get_buffer()
{
   return (char*)malloc( WEBSERVER_MAX_RESPONSE_SIZE );
}

void request_set(const char* what)
{
  
   std::string url = std::string( "/set/" ) + std::string( CONFIG.password ) + std::string( "/" ) + std::string(what);
   WEBSERVER._test_serve( url.c_str() );
}


TEST_CASE( "Main test", "[main]" ) 
{
   WEBSERVER._test_clear();
   RESET_FAKE( Logger__setup_fatal_hook );
   Platform_ESP8266__connected_fake.return_val = true;
   ntp_update_fake.return_val = 1 << 20;
   setup();
   
   Device__jsonify_fake.custom_fake = dev_jsonify;
   webserver_get_buffer_fake.custom_fake = get_buffer;
   
   
   SECTION("serve devices status")
   {
      WEBSERVER._test_serve("/get/dev");
      REQUIRE( WEBSERVER._test_sent_code == 200 );
      std::cout << WEBSERVER._test_sent_content << "\n";
   }
   
   SECTION("password ntp no wifi")
   {
      Platform_ESP8266__connected_fake.return_val = false;
      request_set("ntp");
      REQUIRE( WEBSERVER._test_sent_code == 500 ); 
   }   
   
   
   SECTION("password ntp ok")
   {
      request_set("ntp");
      REQUIRE( WEBSERVER._test_sent_code == 200 ); 
   }
      
   SECTION("password ntp fail")
   {
      ntp_update_fake.return_val = 0;
      request_set("ntp");
      REQUIRE( WEBSERVER._test_sent_code == 500 ); // returns zero == not valid
   }

   /*
   SECTION("password email ")
   {
      ntp_update_fake.return_val = 0;
      email_send_fake.custom_fake = check_email;
      request_set("email");
      REQUIRE( WEBSERVER._test_sent_code == 200 ); // returns zero == not valid
   }*/
   
   SECTION("email hook")
   {
      REQUIRE( Logger__setup_fatal_hook_fake.call_count == 1);
      email_send_fake.custom_fake = check_email;
      Platform_ESP8266__connected_fake.return_val = true;
      
      LoggerFatalHook hook = Logger__setup_fatal_hook_fake.arg1_history[0];
      hook( "FATAL LOG LINE");
   }
   
   SECTION("loop")
   {
      loop();
   }
}
   