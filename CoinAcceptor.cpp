#include "CoinAcceptor.h"
#include "PinManager.h"
#include <Arduino.h>

using Coins = ccTalk::Coins;

CoinAcceptor::CoinAcceptor() {
  coin_serial = new SoftwareSerial(PinManager::PIN_CCTALK_RX, PinManager::PIN_CCTALK_TX);
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
}

uint8_t CoinAcceptor::get_current_coin() {
  return current_coin;
}

bool CoinAcceptor::coin_is_valid() {
  return (current_coin != Coins::INVALID_COIN);
}

bool CoinAcceptor::coin_pig_1() {
  return ((current_coin == Coins::CHF_010) || (current_coin == Coins::CHF_020) || (current_coin == Coins::CHF_050));
}

bool CoinAcceptor::coin_pig_2() {
  return ((current_coin == Coins::CHF_100) || (current_coin == Coins::CHF_200));
}

bool CoinAcceptor::coin_pig_3() {
  return ((current_coin == Coins::CHF_500));
}

void CoinAcceptor::update_state() {
  current_coin = cctalk->read_coin();
}