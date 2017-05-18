


#include <Arduino.h>
#include "serial.h"

static const int BUFFER_SIZE = 64;
static char BUFFER[BUFFER_SIZE];

void serial_setup( const char* name, int baudrate )
{
  Serial.begin( baudrate ); 
  Serial.write("***********************************\n");  
  serial_print("Welcome to %s!\n", name );
  Serial.write("***********************************\n");
}


bool serial_process_number( const char* buffer, int buffer_n, int* convert )
{
    char* end_ptr;
    int value = strtol( (const char*)buffer, &end_ptr, 10  );
    if ( end_ptr != buffer + buffer_n )
    {
        Serial.write(" Invalid: '" );
        Serial.write( (const char*)buffer);
        Serial.write("'\n"); 
        return false;
    }
    
    *convert = value;
    return true;
}



char* serial_receive( int* buffer_len)
{
  
  static int loop = 0;
  
  // if there's any serial available, read it:
  while (Serial.available() > 0) 
  {
    // do it again:
    BUFFER[ loop ] = Serial.read();

    if (BUFFER[loop] == '\n')
    {
      BUFFER[loop] = 0x00;
      *buffer_len = loop;
      loop = 0;
      return BUFFER;
    }

    loop += 1;
    if ( loop >= 64 )
    {
      Serial.write("E: Too long line\n");
      loop = 0;
    }
    
  }
  return NULL;
}

void serial_print_raw(const char* buffer, int len, bool linechange)
{
   Serial.write( buffer, len );
   if ( linechange )
   {
      Serial.write( "\n", 1 );
   }
}


void serial_print( const char* format, ... )
{
   va_list arg_list;
   va_start(arg_list, format);
   memset( BUFFER, 0x00, sizeof(BUFFER));
   int ret = vsnprintf( BUFFER,  BUFFER_SIZE, format, arg_list );
   va_end(arg_list);
   Serial.write( BUFFER, ret ); 
}

int serial_receive_number(int min_value, int max_value)
{
   int buffer_n = 0;
   int number;
   bool print_prompt = true;
   
   while ( true ) 
   {
      if ( print_prompt )
      {
        Serial.write(">");
        print_prompt = false;
      }
      char* buffer = serial_receive( &buffer_n );
      if ( buffer == NULL )
        continue;
      Serial.write(buffer);
      Serial.write("\n");
     
      print_prompt = true;
      
      if ( serial_process_number( buffer, buffer_n , &number ) == false )
        continue;
      
      if ( number < min_value )
      {
         serial_print("Too small number. Min %d\n", min_value );
      }   
      else if ( number > max_value)
      {
         serial_print("Too large number. Max %d\n", max_value );
      }
      else
      {
         return number;
      }
   }
}
