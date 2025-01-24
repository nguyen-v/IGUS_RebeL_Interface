#include "PinManager.h"
#include <Arduino.h>

static Adafruit_MCP23X17 PinManager::mcp;

void PinManager::setup_pins() {

  // Default address with A0..2 to GND is 0x20
  if (!mcp.begin_I2C()) {
    Serial.println("MCP23017 not found...");
    while (1);
  }

  // MCP23017 ===============================================
  // Configure input pins. There is an external 100k pull-up
  // This means that digital HIGH inputs will be seen as LOW
  // and digital LOW as HIGH.
  mcp.pinMode(PIN_FAULT_IN, INPUT);
  mcp.pinMode(PIN_DATA0_IN, INPUT);
  mcp.pinMode(PIN_DATA1_IN, INPUT);
  mcp.pinMode(PIN_DATA2_IN, INPUT);
  mcp.pinMode(PIN_DATA3_IN, INPUT);
  mcp.pinMode(PIN_ACK_IN, INPUT);

  // Outputs
  mcp.pinMode(PIN_ENABLE_OUT, OUTPUT);
  mcp.pinMode(PIN_DATA0_OUT, OUTPUT);
  mcp.pinMode(PIN_DATA1_OUT, OUTPUT);
  mcp.pinMode(PIN_DATA2_OUT, OUTPUT);
  mcp.pinMode(PIN_DATA3_OUT, OUTPUT);
  mcp.pinMode(PIN_DATA4_OUT, OUTPUT);
  mcp.pinMode(PIN_ACK_OUT, OUTPUT);

  // Setup interrupts
  // No mirroring, open-drain, interrupts active LOW
  mcp.setupInterrupts(false, true, LOW);
  mcp.setupInterruptPin(PIN_ACK_IN, LOW);
  mcp.setupInterruptPin(PIN_FAULT_IN, LOW);

  // Arduino Nano Every ======================================
  pinMode(PIN_INTA_FAULT_IN, INPUT_PULLUP);
  pinMode(PIN_INTB_ACK_IN, INPUT_PULLUP);

  pinMode(PIN_SOLENOID, OUTPUT);

  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);

  pinMode(PIN_SENS_UP, INPUT_PULLDOWN);
  pinMode(PIN_SENS_DW, INPUT_PULLDOWN);
}
