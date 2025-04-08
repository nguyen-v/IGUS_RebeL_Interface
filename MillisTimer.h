#ifndef MILLIS_TIMER_H
#define MILLIS_TIMER_H

class MillisTimer
{
  public:
    void startt(int tdelay); // start the countdown with tdelay in miliseconds
    bool isready(void);// return true if timer expired
    void disable(void);
  private:
    unsigned long target;
    bool enabled = false;
};

#endif