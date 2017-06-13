#ifndef SUPA_STIMER_H
#define SUPA_STIMER_H


class STimer
{
  public:
    STimer();
    /// Use all time
    void reset(); 
    /// Use @param timeout_ms amount of time 
    void reset_with_carry( unsigned long timeout_ms ); 
    /// check if @param timeout_ms milliseconds has passed since last reset.
    bool check( unsigned long timeout_ms ); 
    /// @returns time since last reset
    unsigned long elapsed(); 
  protected:
    unsigned long reset_ms;
};



#endif