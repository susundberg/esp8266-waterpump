#define MODULE_NAME "Distance"

#include <Arduino.h>

#include "distance.h"

#include "logger.h"


void Distance::setup(int pin_trigger, int pin_echo)
{
   this->pin_echo = pin_echo;
   this->pin_trigger = pin_trigger;
   pinMode(pin_trigger, OUTPUT);
   pinMode(pin_echo, INPUT);
}

static const int maximumRange = 200; // Maximum range needed
static const int minimumRange = 0; // Minimum range needed

int Distance::get_distance()
{
 digitalWrite(this->pin_trigger, LOW); 
 delayMicroseconds(2); 

 digitalWrite(this->pin_trigger, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(this->pin_trigger, LOW);
 long duration = pulseIn(this->pin_echo, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 int distance = (duration + (58/2))/58;
 
 LOG_INFO("Distance: %d %ld", distance, duration );
 if (distance >= maximumRange || distance <= minimumRange)
 {
    return -1;
 }
 else 
 {
    return distance;
 }
}
