#ifndef COIN_ACCEPTOR_H
#define COIN_ACCEPTOR_H

#include <SoftwareSerial.h>
#include "ccTalk.h"

class CoinAcceptor {
public:
  CoinAcceptor::CoinAcceptor();
  void init();
  void update_state();
  uint8_t get_current_coin();
  bool coin_is_valid();
  bool coin_pig_1();
  bool coin_pig_2();
  bool coin_pig_3();

private:
  SoftwareSerial* coin_serial;
  ccTalk* cctalk;
  int credit;
  uint8_t current_coin;
};

#endif