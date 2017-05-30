/**
*/

#include "custom_logger.h"

Logger LOG;

bool _TEST_log_verbose = false;


void _TEST_log_hookup()
{
   Log__log.custom_fake = _TEST_log;
}

void _TEST_log(Logger::Level level, const __FlashStringHelper* format, ... )  
{

   va_list argument_list;
   va_start ( argument_list, format );       

   
   
   if (_TEST_log_verbose == false )
   {
      char output_buffer[256];
      // Make sure logging troubles get caugth by valgrind
      vsnprintf( output_buffer, 256, format, argument_list );
   }
   else
   {
      printf("LOG %d:", (int)level );
      vprintf( format, argument_list );
      printf("\n");
   }
   
   va_end( argument_list );
   
}

