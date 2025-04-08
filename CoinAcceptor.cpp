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

  insertion_counter = cctalk->get_insertion_counter();
  Serial.println(F("Initial insertion counter: "));
  Serial.print(insertion_counter);
  Serial.println();
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
  return ((current_coin == Coins::CHF_010) || (current_coin == Coins::CHF_020));
}

bool CoinAcceptor::coin_pig_2() {
  return ((current_coin == Coins::CHF_200) || (current_coin == Coins::CHF_500));
}

bool CoinAcceptor::coin_pig_3() {
  return ((current_coin == Coins::CHF_100) || (current_coin == Coins::CHF_050));
}

bool CoinAcceptor::update_state() {
  // Serial.println("loop entry");
  // for (uint8_t i = 0; i < 5; ++i) {
  uint8_t temp_coin = cctalk->read_coin();
  if ((temp_coin != Coins::INVALID_COIN) && (temp_coin != Coins::NO_COINS)) {
    current_coin = temp_coin;
    ccTalk::print_coin_value(current_coin);
    Serial.println("return");
    return true;
  }
  // // delay(10);
  // // temp_coin = cctalk->read_coin();
  // // delay(50);
  int current_reject_counter = cctalk->get_reject_counter();
  // // delay(50);
  // int current_insertion_counter = cctalk->get_insertion_counter();
  // delay(10);
  // }
  // delay(50);
  // temp_coin = cctalk->read_coin();
  // int num_new_coins = current_insertion_counter - insertion_counter;
  // Serial.print(F("Insertion counter new: "));
  // Serial.print(current_insertion_counter);
  // Serial.print(F(" old: "));
  // Serial.print(insertion_counter);
  // Serial.println();

  // if (num_new_coins > 0 and num_new_coins <= 2) { // we check for <= 2 to avoid garbage values
  //   num_new_coins = 0;
  //   insertion_counter = current_insertion_counter;
  //   Serial.println("Counter increased");
  //   current_coin = cctalk->read_coin();
  //   ccTalk::print_coin_value(current_coin);
  //   return true;
  // }
  // return false;
    if (current_reject_counter > reject_counter) {
      reject_counter = current_reject_counter;
      current_coin = Coins::INVALID_COIN;
      ccTalk::print_coin_value(current_coin);
      Serial.println("return");
      return true;
    }
  return false;
}

void CoinAcceptor::reset_state() {
  do {
    insertion_counter = cctalk->get_insertion_counter();
  } while (insertion_counter == 0);
  current_coin = Coins::INVALID_COIN;
}