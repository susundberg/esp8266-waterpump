#ifndef SUPA_STIMER_H
#define SUPA_STIMER_H


class STimer
{
  public:
    STimer();
    void reset();
    bool check( unsigned long timeout );
   
  protected:
    unsigned long reset_ms;
};



#endif