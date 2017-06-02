#include <WiFiClient.h>


#include "config.h"
#include "push_thingspeak.h"
#include "logger.h"


Push_thingspeak::Push_thingspeak()
{
   this->timer.reset();
}

bool Push_thingspeak::thingspeak_push_raw(const Device_input** values, int values_n, char* buffer )
{
   WiFiClient client;
   if ( client.connect( CONFIG.push.host, 80 ) == false )
   {
     LOG_WARN("Cannot connect '%s'", CONFIG.push.host );
     return false;
   }
   LOG_INFO("Push data to '%s'", CONFIG.push.host );
   
   // cbuffer "https://api.thingspeak.com/update?api_key=<api key>&field1=0&field2=12"
   int buffer_offset = snprintf( buffer, buffer_size, "GET /update?api_key=%s", CONFIG.push.api_key );
   int ret;
   
   for ( int loop = 0; loop < values_n; loop ++ )
   {
      const Device_input* dev = values[loop];
      ret = snprintf( buffer + buffer_offset, buffer_size - buffer_offset, "&field%d=%d", (loop+1), dev->get_value() );
      if ( ret >= buffer_size - buffer_offset )
         return false;
      buffer_offset += ret;
   }
   ret = snprintf( buffer + buffer_offset, buffer_size - buffer_offset, 
                   " HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", CONFIG.push.host );
   if ( ret >= buffer_size - buffer_offset )
      return false;
   buffer_offset += ret;
   client.write( (const uint8_t *)buffer, buffer_offset );
   Serial.write( (const char *)buffer, buffer_offset );
   client.stop();
   return true;
}

   
   
    

bool Push_thingspeak::thingspeak_push(const Device_input** values, int values_n, bool force_update )
{
   
   if ( force_update == false && timer.check( push_interval_s*1000 ) == false )
      return false;
   
   timer.reset();
   
   char* buffer = (char*)malloc(buffer_size);
   
   if ( buffer == NULL )
      return false;
   
   bool ret = thingspeak_push_raw(values, values_n, buffer );
   free( buffer );
   return ret;
}

