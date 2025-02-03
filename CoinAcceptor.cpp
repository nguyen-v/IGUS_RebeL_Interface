#include "CoinAcceptor.h"
#include "PinManager.h"
#include <Arduino.h>

using Coins = ccTalk::Coins;

CoinAcceptor::CoinAcceptor() {
  // coin_serial = new AltSoftSerial(); // by default uses pins 8, 9
  coin_serial = &Serial1; // Hardware serial (by default D0, D1)
  cctalk = new ccTalk(coin_serial);
  current_coin = Coins::INVALID_COIN;
}

void CoinAcceptor::init() {
  coin_serial->begin(9600);
  cctalk->comm_init();

  while ( cctalk->RX_state != ccTalk::RXidle) {
    cctalk->ccTalkReceive();
  }

  cctalk->device_init();

  Serial.println(F("Getting inhibit status"));
  Serial.println(cctalk->get_inhibit());

  Serial.println(F("Enabling inhibit"));
  cctalk->inhibit_on();
  Serial.println(F("Getting inhibit status"));
  Serial.println(cctalk->get_inhibit());

  cctalk->inhibit_off();
  Serial.println(F("Ready to accept coins"));

  update_state();
}

uint8_t CoinAcceptor::get_current_coin() {
  return current_coin;
}

bool CoinAcceptor::coin_is_valid() {
  return ((current_coin != Coins::INVALID_COIN) && (current_coin != Coins::NO_COINS));
}

bool CoinAcceptor::coin_is_present() {
  return (current_coin != Coins::NO_COINS);
}

bool CoinAcceptor::coin_pig_1() {
  return ((current_coin == Coins::CHF_010) || (current_coin == Coins::CHF_020) || (current_coin == Coins::CHF_050));
}

bool CoinAcceptor::coin_pig_2() {
  return ((current_coin == Coins::CHF_500));
}

bool CoinAcceptor::coin_pig_3() {
  return ((current_coin == Coins::CHF_100) || (current_coin == Coins::CHF_200));
}

bool CoinAcceptor::update_state() {
  // Serial.println("loop entry");
  // for (uint8_t i = 0; i < 5; ++i) {
  uint8_t temp_coin = cctalk->read_coin();
  if ((temp_coin != Coins::INVALID_COIN) && (temp_coin != Coins::NO_COINS))
    current_coin = temp_coin;
  // // delay(10);
  // // temp_coin = cctalk->read_coin();
  // // delay(50);
  // int current_reject_counter = cctalk->get_reject_counter();
  // // delay(50);
  int current_insertion_counter = cctalk->get_insertion_counter();
  // delay(10);
  // }
  // delay(50);
  // temp_coin = cctalk->read_coin();

  if (current_insertion_counter > insertion_counter) {
    insertion_counter = current_insertion_counter;
    return true;
  }
  return false;
  //   if (current_reject_counter > reject_counter) {
  //     reject_counter = current_reject_counter;
  //     current_coin = Coins::INVALID_COIN;
  //   } else {
  //     current_coin = temp_coin;
  //   }
  //   ccTalk::print_coin_value(current_coin);
  // }
}

void CoinAcceptor::reset_state() {
  current_coin = Coins::INVALID_COIN;
}