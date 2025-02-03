#ifndef COIN_ACCEPTOR_H
#define COIN_ACCEPTOR_H

// #include <AltSoftSerial.h>
#include "ccTalk.h"

class CoinAcceptor {
public:
  CoinAcceptor();
  void init();
  bool update_state();
  uint8_t get_current_coin();
  bool coin_is_valid();
  bool coin_is_present();
  void reset_state();
  bool coin_pig_1();
  bool coin_pig_2();
  bool coin_pig_3();

private:
  // Stream* coin_serial;
  HardwareSerial* coin_serial;
  ccTalk* cctalk;
  uint8_t current_coin;
  int reject_counter;
  int insertion_counter;
};

#endif