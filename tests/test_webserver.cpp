#include <iostream>


#include "catch.hpp"
#include "Arduino.h"
#include "mock_logger.h"

#include "webserver.h"

static void test_reset()
{
  WEBSERVER._test_clear(); 
  webserver_setup();
}

TEST_CASE( "Webserver basic fun", "[webserver]" ) 
{
  test_reset();
  webserver_loop();
}


TEST_CASE( "Webserver status", "[webserver]" ) 
{
  test_reset();  
  
  REQUIRE( WEBSERVER._test_serve("/get/status") == true );
  REQUIRE( WEBSERVER._test_sent_code == 200 );
  std::cout << WEBSERVER._test_sent_content << "\n"; 
  webserver_loop();
}

TEST_CASE( "Webserver log", "[webserver]" ) 
{
  test_reset();   
  REQUIRE( WEBSERVER._test_serve("/get/log") == true );
  REQUIRE( WEBSERVER._test_sent_code == 200 );
  std::cout << WEBSERVER._test_sent_content << "\n"; 
  // then put some content
  const char* log_lines[] = { "This is log line1", "This is log line2", "This is log line3", NULL };
  
  SET_RETURN_SEQ( Logger__get_log_line, log_lines, 4);    
  REQUIRE( WEBSERVER._test_serve("/get/log") == true );
  REQUIRE( WEBSERVER._test_sent_code == 200 );
  std::cout << WEBSERVER._test_sent_content << "\n"; 

   
}
