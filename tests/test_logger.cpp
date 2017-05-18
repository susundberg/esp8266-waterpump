
#include "catch.hpp"

#define MODULE_NAME "FOOBAR"

#include "logger.h"
#include <iostream>
#include <string.h>
#include "Arduino.h"

#include "mock_stimer.h"



TEST_CASE( "Logging ", "[logger]" ) 
{
   LOG = Logger();
   millis_fake.return_val = 1000*( 45 + 15*60 + 8*60*60 + 1*60*60*24);
   RESET_FAKE( digitalWrite );
   
   SECTION("Basic setup")
   {
      LOG.setup_serial( "TESTHOST", 115200 );
      LOG.setup_led( 13 );
      
      STimer__check_fake.return_val = true;
      
      LOG_INFO("STARTED");
      
      for ( int loop = 0; loop < 10; loop ++ )
         LOG.loop();
      
      REQUIRE( digitalWrite_fake.call_count == 11 );
      
      LOG.set_status( Logger::Status::RUNNING );

      for ( int loop = 0; loop < 10; loop ++ )
      {
         LOG.loop();
      }
      REQUIRE( digitalWrite_fake.call_count == 21  );
   }
   
   SECTION("Basic logging")
   {
      REQUIRE( LOG.get_log_line(0) == NULL  );
      
      for ( int loop = 0; loop < 100; loop ++ )
      {
         LOG_INFO("LOOPING IS FUN %03d" , loop );
      }
      std::string last_line = std::string ( LOG.get_log_line(0) );
      REQUIRE( last_line == "[32:15:45] I:LOOPING IS FUN 099" );

      last_line = std::string ( LOG.get_log_line(2) );
      REQUIRE( last_line == "[32:15:45] I:LOOPING IS FUN 097" );
   }

   SECTION("Logging errors")
   {
   
      LOG_INFO("THIS IS OK" );
      LOG_ERROR("THIS IS SOOOOO LONG ERROR THAT I DONT HANDLE IT THIS IS SOOOOO LONG ERROR THAT I DONT HANDLE IT THIS IS SOOOOO LONG ERROR THAT I DONT HANDLE IT THIS IS SOOOOO LONG ERROR THAT I DONT HANDLE IT THIS IS SOOOOO LONG ERROR THAT I DONT HANDLE IT");
      
      const char* str = LOG.get_log_line(1);
      int len = strlen( str );
      REQUIRE( len < 100 );
      REQUIRE( LOG.get_status() == Logger::Status::ERROR );
      REQUIRE( std::string( LOG.get_log_line(1) ) == "[32:15:45] I:THIS IS OK" );
      LOG.loop();
   }

   SECTION( "Logging levels") 
   {
      LOG = Logger();
      
      LOG_ERROR("OK");
      LOG_WARN("OK" );
      REQUIRE( std::string( LOG.get_log_line(0) ) == "[32:15:45] W:OK" );
      REQUIRE( std::string( LOG.get_log_line(1) ) == "[32:15:45] E:OK" );
   }
}   