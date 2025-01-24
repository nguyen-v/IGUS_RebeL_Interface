#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#include <Arduino.h>
#include <Adafruit_MCP23X17.h>

class PinManager {
public:
  void setup_pins();

  // MCP23017 GPIO Expander =============================================

  // MCP23017 GPA0..5
  static const int PIN_FAULT_IN = 0;  // Fault    IN: Dout21 (interrupt)
  static const int PIN_DATA0_IN = 1;  // Data[0]  IN: Dout22
  static const int PIN_DATA1_IN = 2;  // Data[1]  IN: Dout23
  static const int PIN_DATA2_IN = 3;  // Data[2]  IN: Dout24
  static const int PIN_DATA3_IN = 4;  // Data[3]  IN: Dout25
  static const int PIN_ACK_IN   = 5;  // ACK      IN: Dout27 (interrupt)

  // MCP23017 GPB0..6
  static const int PIN_ENABLE_OUT =  8;   // Enable   OUT:  Din21
  static const int PIN_DATA0_OUT  =  9;   // Data[0]  OUT:  Din22
  static const int PIN_DATA1_OUT  =  10;  // Data[1]  OUT:  Din23
  static const int PIN_DATA2_OUT  =  11;  // Data[2]  OUT:  Din24
  static const int PIN_DATA3_OUT  =  12;  // Data[3]  OUT:  Din25
  static const int PIN_DATA4_OUT  =  13;  // Data[4]  OUT:  Din26
  static const int PIN_ACK_OUT    =  14;  // ACK      OUT:  Din27

  // Arduino Nano Every ==================================================

  // Interrupts for FAULT_IN and ACK_IN
  static const int PIN_INTA_FAULT_IN = 4;
  static const int PIN_INTB_ACK_IN = 5;

  // UART (Software Serial). These are configured in the relevant classes.
  static const int PIN_HAND_TX = 13;
  static const int PIN_HAND_RX = 14;

  // Coin detector
  static const int PIN_CCTALK = 8;

  // Solenoid
  static const int PIN_SOLENOID = 3;

  // LEDs
  static const int PIN_LED_R = A2;
  static const int PIN_LED_G = A3;

  // Sensors
  static const int PIN_SENS_UP = A0;
  static const int PIN_SENS_DW = A1;

  static Adafruit_MCP23X17 mcp;
};

#endif