
#ifndef SUPA_DEV_PIN_OUT_H
#define SUPA_DEV_PIN_OUT_H

#include <stdint.h>

#include "device.h"

class Device_pin_out : public Device_output
{
public:
   Device_pin_out( const char* name, uint8_t pin, uint8_t value);
   virtual void setup(  ) override;  
   virtual void set_value( int value ) override;  
   
protected:
   uint8_t pin ;
};


#endif