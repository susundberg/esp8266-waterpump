#include <iostream>

#include "catch.hpp"
#include "email_send.h"

#include "WiFiClientSecure.h"


static void reply_invalid( WiFiClientSecure* client )
{
   client->_test_input.push_front( String("Invalid reply comes from here, but we do have linechange\n"));
}

static void reply_proper( WiFiClientSecure* client )
{
   
   const std::string replies[] = { 
      String("220 smtp.gmail.com ESMTP s81sm971317lja.7 - PART1"),
      String("PART2\n"),
      String("250-smtp.gmail.com at your service, [87.100.197.21]250-SIZE 35882577\n\
250-8BITMIME\n\
250-AUTH LOGIN PLAIN XOAUTH2 PLAIN-CLIENTTOKEN OAUTHBEARER XOAUTH\n\
250-ENHANCEDSTATUSCODES\n\
250-PIPELINING\n\
250-CHUNKING\n\
250 SMTPUTF8\n\n"),
      String("334 VXNlcm5hbWU6\n"),
      String("334 UGFzc3dvcmQ6\n"),
      String("235 2.7.0 Accepted\n"),
      String("250 2.1.0 OK s81sm971317lja.7 - gsmtp\n"),
      String("250 2.1.5 OK s81sm971317lja.7 - gsmtp\n"),
      String("354  Go ahead s81sm971317lja.7 - gsmtp\n"),
      String("250 2.1.5 bye bye\n") };

  for ( unsigned int loop = 0; loop < sizeof(replies)/sizeof(std::string ); loop ++ )
  {
     client->_test_input.push_front( replies[loop] );
  }
  
  // client->_test_verbose = true;
   
}

TEST_CASE( "Email", "[email]" ) 
{
   EmailSettings settings = { .login = "login@gmail.com", .password="password", .sender = "sender@gmail.com", .server_host = "smtp.google.com", .server_port = 465 };
   
   SECTION("No response")
   {
      bool ret = email_send( &settings, "receiver@test.com", "Subject here", "Message body goes here" );
      REQUIRE( ret == false );
   }

   SECTION("Invalid response")
   {
      WiFiClientSecure::_test_create_hook = reply_invalid;
      bool ret = email_send( &settings, "receiver@test.com", "Subject here", "Message body goes here" );
      REQUIRE( ret == false );
   }

   SECTION("Proper response")
   {
      WiFiClientSecure::_test_create_hook = reply_proper;
      bool ret = email_send( &settings, "receiver@test.com", "Subject here", "Message body goes here" );
      REQUIRE( ret == true );
   }

   
}
