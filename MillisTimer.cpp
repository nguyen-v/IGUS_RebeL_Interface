#include "Arduino.h"
#include "MillisTimer.h"

void MillisTimer::startt(int tdelay) {// start the timer witth delay in miliseconds. 
  enabled = true;
  unsigned long temp;
  temp = millis();
  target = temp + tdelay;
}

void MillisTimer::disable(void) {
  enabled = false;
}

bool MillisTimer::isready(void) {//return true if timer is expired 
  unsigned long temp;
  temp = millis();
  if ((target - temp > 86400000) && enabled) // one day
    return true;
  else
    return false;
}