#include "PinManager.h"
#include <Arduino.h>

void PinManager::setup_pins() {
  // Configure input pins

  // Uno needs external pulldowns
  pinMode(PIN_FAULT_IN, INPUT);
  pinMode(PIN_DATA0_IN, INPUT);
  pinMode(PIN_DATA1_IN, INPUT);
  pinMode(PIN_DATA2_IN, INPUT);
  pinMode(PIN_DATA3_IN, INPUT);
  pinMode(PIN_ACK_IN, INPUT);

  // Nano Every can use its internal pulldowns
  // pinMode(PIN_FAULT_IN, INPUT_PULLDOWN);
  // pinMode(PIN_DATA0_IN, INPUT_PULLDOWN);
  // pinMode(PIN_DATA1_IN, INPUT_PULLDOWN);
  // pinMode(PIN_DATA2_IN, INPUT_PULLDOWN);
  // pinMode(PIN_DATA3_IN, INPUT_PULLDOWN);
  // pinMode(PIN_ACK_IN, INPUT_PULLDOWN);

  // Outputs
  pinMode(PIN_ENABLE_OUT, OUTPUT);
  pinMode(PIN_DATA0_OUT, OUTPUT);
  pinMode(PIN_DATA1_OUT, OUTPUT);
  pinMode(PIN_DATA2_OUT, OUTPUT);
  pinMode(PIN_DATA3_OUT, OUTPUT);
  pinMode(PIN_DATA4_OUT, OUTPUT);
  pinMode(PIN_ACK_OUT, OUTPUT);
}
