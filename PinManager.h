#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#include <Arduino.h>

class PinManager {
public:
    void setup_pins();

    static const int PIN_FAULT_IN = 2;  // Fault    IN: Dout21 (interrupt)
    static const int PIN_DATA0_IN = A0; // Data[0]  IN: Dout22
    static const int PIN_DATA1_IN = A1; // Data[1]  IN: Dout23
    static const int PIN_DATA2_IN = A2; // Data[2]  IN: Dout24
    static const int PIN_DATA3_IN = A3; // Data[3]  IN: Dout25
    static const int PIN_ACK_IN   = 3;  // ACK      IN: Dout27 (interrupt)

    static const int PIN_ENABLE_OUT =  7;  // Enable   OUT:  Din21
    static const int PIN_DATA0_OUT  =  8;  // Data[0]  OUT:  Din22
    static const int PIN_DATA1_OUT  =  9;  // Data[1]  OUT:  Din23
    static const int PIN_DATA2_OUT  =  10; // Data[2]  OUT:  Din24
    static const int PIN_DATA3_OUT  =  11; // Data[3]  OUT:  Din25
    static const int PIN_DATA4_OUT  =  12; // Data[4]  OUT:  Din26
    static const int PIN_ACK_OUT    =  13; // ACK      OUT:  Din27

    // UART (Software Serial). These are configured in the relevant classes.
    static const int PIN_HAND_TX = 4;
    static const int PIN_HAND_RX = 5;

    static const int COIN_TX = A4;
    static const int COIN_RX = A5;
};

#endif