#include "PinManager.h"
#include <Arduino.h>

Adafruit_MCP23X17 PinManager::mcp;

void PinManager::setup_pins() {

  // Default address with A0..2 to GND is 0x20
  if (!mcp.begin_I2C()) {
    Serial.println("MCP23017 not found...");
    while (1);
  }
  Serial.println("MCP23017 found...");

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
  mcp.pinMode(PIN_PROG_OUT, OUTPUT);

  // Arduino Nano Every ======================================
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_INTA_FAULT_IN, INPUT_PULLUP);
  pinMode(PIN_INTB_ACK_IN, INPUT_PULLUP);

  // Solenoid
  pinMode(PIN_SOLENOID, OUTPUT);
  // Set the PWM frequency to 31.37255 kHz (not audible)
  TCB1_CTRLA = 0b00000011;  // pin D3

  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
}

bool PinManager::read_sensor_up() {
  return (analogRead(PIN_SENS_UP) < sensor_thr);
}

bool PinManager::read_sensor_down() {
  return (analogRead(PIN_SENS_DW) > sensor_thr);
}

void PinManager::open_solenoid() {
  analogWrite(PIN_SOLENOID, 255);
  delay(100);
  analogWrite(PIN_SOLENOID, solenoid_pwm);
}

void PinManager::close_solenoid() {
  analogWrite(PIN_SOLENOID, 0);
}
