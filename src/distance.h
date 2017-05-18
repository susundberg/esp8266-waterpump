

#ifndef SUPA_DISTANCE_H
#define SUPA_DISTANCE_H



class Distance
{
  public:
      void setup( int pin_trigger, int pin_echo );
      int get_distance();
      
  protected:
    int pin_trigger;
    int pin_echo;
    
};


#endif