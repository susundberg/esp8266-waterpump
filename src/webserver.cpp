#include <string.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#include "webserver.h"
#include "logger.h"

ESP8266WebServer WEBSERVER(80);

char* webserver_get_buffer()
{
  char* buffer = (char*)malloc( WEBSERVER_MAX_RESPONSE_SIZE );
  if (buffer == NULL )
  {
     WEBSERVER.send(500, "text/plain", "Error!");
     return NULL;
  }
  return buffer;
}


 
static void handle_log() 
{
  char* buffer = webserver_get_buffer();

  if ( buffer == NULL )
     return;
  
  int len = 0;
  int total_len = 0;
  for(int loop = 0; loop < 100; loop ++ ){ buffer[loop] = 'F'; };
  
  const char* json_start = "{\"log\":[";
  len = strlen(json_start);
  
  memcpy( buffer, json_start, len + 1 );
  total_len += len;
  
  for ( int loop = 0; loop < Logger::max_lines; loop ++ )
  {
     const char* log_line = LOG.get_log_line( loop );
     
     if ( log_line == NULL )
        break;

     if ( loop > 0 )
     {
        strncat( buffer + total_len, ",\"", 2 );
        total_len += 2;
     }
     else
     {
        strncat( buffer + total_len, "\"", 1 );
        total_len += 1;
     }
     
     len = strlen( log_line );
     strncat( buffer + total_len, log_line, len );
     total_len += len;
     
     strncat( buffer + total_len, "\"\n", 2 );
     total_len += 1;
  }
  
  const char* json_stop = "]}\n";
  strncat( buffer, json_stop, 3 );
  total_len += 3;
  
  WEBSERVER.send( 200, "application/json", buffer );
  free(buffer);
}


static void handle_status()
{
   char* buffer = webserver_get_buffer();
   
   const char* status ;
   
   if ( LOG.get_status() == Logger::Status::RUNNING )
   {
      status = "Running";
   }
   else
   {
      status = "Error";
   }
   
   
   InfoUptime uptime;
   memset(&uptime, 0x00, sizeof(uptime));
   get_uptime(&uptime);
   
   
   snprintf( buffer, WEBSERVER_MAX_RESPONSE_SIZE, "{ \"status\":\"%s\",\"hour\":%u,\"min\":%u,\"sec\":%u}", 
             status, uptime.hours, uptime.minutes, uptime.seconds  );
   
   WEBSERVER.send( 200, "application/json", buffer );
   
   free(buffer);
}

void webserver_setup()
{
  WEBSERVER.on( "/get/log", handle_log );
  WEBSERVER.on( "/get/status", handle_status );
  WEBSERVER.serveStatic("/", SPIFFS, "/index.html", "max-age=86400");
  WEBSERVER.serveStatic("/data/", SPIFFS, "/", "max-age=86400");
  WEBSERVER.onNotFound([](){ WEBSERVER.send(404, "text/plain", "Not found"); });
  WEBSERVER.begin();
}

void webserver_loop(void)
{
   WEBSERVER.handleClient();
}
