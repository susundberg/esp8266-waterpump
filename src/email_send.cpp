



#include <WiFiClientSecure.h>
#include <base64.h>
#include <Arduino.h>

#define MODULE_NAME "email_send"

#include "email_send.h"
#include "logger.h"

#define FSTR 
  
bool read_response(WiFiClientSecure* client, char* buffer, int buffer_len, const char* endmarker, const char* exp_response, const char* log_string )
{
   
  int buffer_offset  = 0;
  bool line_change_found = false;
  int endmarker_len = strlen(endmarker);
  int delay_loop_max = EMAIL_SEND_TIMEOUT_MS/10;
  int delay_loop = 0;
  
  // make sure we start with 0 sized string
  memset( buffer, 0x00, buffer_len );
  while ( ( line_change_found == false ) && (delay_loop < delay_loop_max ))
  {
     int avail = client->available();
     
     
     if (avail == 0)
     {
        delay(10);
        delay_loop += 1;
        continue;
     }
     
     int to_read = min( avail, (buffer_len - buffer_offset));
     int red = client->read( (uint8_t*)(buffer + buffer_offset), to_read );
     
     buffer[ buffer_offset + red ] = 0;
     int search_start = max( 0, buffer_offset - endmarker_len  );
     
     if ( strstr( buffer + search_start , endmarker ) != NULL )
     {
        line_change_found = true;
     }
         
     buffer_offset += red;
  }
  
  // Uncomment this if you want to see the full response always
  //  Serial.println(buffer);
  int exp_size = strlen(exp_response);
  if ( buffer_offset < exp_size || buffer_offset >= EMAIL_SEND_MAX_SIZE )
  {
     LOG_WARN("Send failed (len) at '%s'", log_string );
     LOG_WARN("Got: '%s'", buffer );
     return false;
  }
  
  if ( strncmp( buffer, exp_response, exp_size ) != 0 )
  {
     LOG_WARN("Send failed (content) at '%s'", log_string );
     LOG_WARN("Got: '%s'", buffer );
     return false;
  }
  return true;
}

bool email_send_raw( const Config_email* settings, const char* receiver, const char* subject, const char* message, 
                     char* buffer, WiFiClientSecure* client )
{

  LOG_INFO( "Sending email via '%s:%d'", settings->server_host, settings->server_port  );
  
  if (!client->connect( settings->server_host, settings->server_port))
  {
     LOG_WARN("Send failed on connect to server");
     return false;
  }

  
  if( read_response( client, buffer, EMAIL_SEND_MAX_SIZE, "\n", FSTR("220"), FSTR("Handshake") ) == false )
     return false;
  
  sprintf( buffer, FSTR("EHLO %s"), client->localIP().toString().c_str() );
  client->println(buffer);
  
  if( read_response( client, buffer, EMAIL_SEND_MAX_SIZE, "\n\n", FSTR("250"), FSTR("Ehlo") ) == false )
     return false;

  sprintf( buffer, FSTR("AUTH LOGIN") );
  client->println(buffer);
  if( read_response( client, buffer, EMAIL_SEND_MAX_SIZE, "\n", FSTR("334"), FSTR("login") ) == false )
     return false;
  
  base64 encoder;
  
  client->println( encoder.encode( settings->login ) );
  if( read_response( client, buffer, EMAIL_SEND_MAX_SIZE, "\n", FSTR("334"), FSTR("login_user") ) == false )
     return false;
  
  client->println( encoder.encode( settings->password ) );
  if( read_response( client, buffer, EMAIL_SEND_MAX_SIZE, "\n", FSTR("235"), FSTR("login_pass") ) == false )
     return false;

  LOG_INFO("Login ok");
  
  sprintf( buffer, FSTR("MAIL FROM: <%s>"), settings->login );
  client->println( buffer );
  
  if( read_response( client, buffer, EMAIL_SEND_MAX_SIZE, "\n", FSTR("250"), FSTR("mail_from") ) == false )
     return false;
  
  sprintf( buffer, FSTR("RCPT TO: <%s>"), receiver );
  client->println( buffer );
  if( read_response( client, buffer, EMAIL_SEND_MAX_SIZE, "\n", FSTR("250"), FSTR("mail_to") ) == false )
     return false;
  
  client->println( FSTR("DATA") );
  if( read_response( client, buffer, EMAIL_SEND_MAX_SIZE, "\n", FSTR("354"), FSTR("data") ) == false )
     return false;

  sprintf( buffer, FSTR("From: <%s>\nTo: <%s>\nSubject: %s\r\n\n%s\r\n.\r\nQUIT"), 
           settings->login, receiver, subject, message );
  client->println( buffer );
  
  if( read_response( client, buffer, EMAIL_SEND_MAX_SIZE, "\n", FSTR("250"), FSTR("quit") ) == false )
     return false;
  
  LOG_INFO("Message sent ok!");
  return true;
}



bool email_send( const Config_email* settings, const char* receiver, const char* subject, const char* message )
{
  static int spam_protection = EMAIL_SEND_MAX_MAILS;
  
  if ( spam_protection <= 0 )
  {
     LOG_WARN("Email '%s' send skipped since quota out.\n");
     return false;
  }
  spam_protection -= 1;
  
  char* buffer = (char*)malloc( EMAIL_SEND_MAX_SIZE ); 
  WiFiClientSecure* client = new WiFiClientSecure();
  
  bool ret = false;
  if ( client == NULL || buffer == NULL )
  {
     LOG_WARN("Out of memory!");
  }
  else
  {
     client->setTimeout(EMAIL_SEND_TIMEOUT_MS);
     ret = email_send_raw( settings, receiver, subject, message, buffer, client );
     client->stop();
  }
  
  delete( client );
  free(buffer);
  
  return ret;
}