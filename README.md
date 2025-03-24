# Arduino code for IGUS ReBeL robot arm

This repository documents the hardware needed to interface an arduino to the IGUS ReBeL robot arm (4 DoFs) through its PLC interface.

## 1. Hardware

- [Arduino Nano Every](https://store.arduino.cc/products/arduino-nano-every)

- [ReBeL cobot 4 DoF](https://www.igus.eu/product/20962)

- [AL66 FG Coin Sorter](https://www.alberici.it/en/product/al66-fg/)

## 2. Pinout

The arduino is connected to the PLC interface through a custom handmade board, on which there are relays/optocouplers and a GPIO multiplexer. The schematic can be found [here](Schematic/igus_interface.pdf).

The pinout is as follows:

### Robot hand (SERVO connector)
It communicates through a software serial interface.
- ```D10``` → ```PIN_HAND_TX```
- ```D12``` → ```PIN_HAND_RX```

### Coin sorter (AL connector)
It communicates through the CCTalk protocol and uses hardware serial 1 (Serial1).
- ```D0``` → ```RX```
- ```D1``` → ```TX```

### LEDs (LEDS connector)
The LEDs are active-high, powered through 5V with a low-side transistor.
- ```A2``` → ```PIN_LED_R```
- ```A3``` → ```PIN_LED_G```

### Solenoid
The solenoid is used to close the coin slot entry when the robot is moving. It is configured as a 31.37255 kHz PWM signal.
- ```D3``` → ```PIN_SOLENOID```

### Interrupts (EXT connector)
- ```D4``` → ```PIN_INTB_ACK_IN```. This pin is active-low and set LOW by the robot arm (in a loop) to acknowledge that it has reached a unique pose (e.g. resting state). Once triggered, the arduino determines the current pose of the arm by using the other outputs that the robot arm sends (see next section).
- ```D2``` → ```PIN_INTA_REF_IN```. This pin is active-low and set LOW by the robot arm once the arm is referenced (once it has finished its startup routine)

### Robot arm (PLC interface) (EXT connector)
The Arduino is interfaced with the robot arm using optocouplers for the inputs (robot arm → Arduino) and relays for the outputs (Arduino → robot arm). To communicate with the arm, we need to send and receive a certain number of data, which indicates either commands to the arm (e.g. go to the pick coin location), or to query the current pose of the arm (e.g. resting pose).

As the Arduino doesn't have enough pins, we use the MCP23017, which is a 16-Bit I2C I/O Expander.

- ```A4``` → ```SDA``` (MCP23017)
- ```A5``` → ```SCL``` (MCP23017)

Note that the pullup resistors are also integrated on the custom board.

The pinout for the MCP23017 is as follows:

```MCP23017 pin``` → ```Pin name``` (PLC I/O)

Outputs (Arduino → robot arm). Used to send commands to the arm.

- ```GPB6``` → ```PIN_ENABLE_OUT``` (Din21)
- ```GPB5``` → ```PIN_DATA0_OUT``` (Din22)
- ```GPB4``` → ```PIN_DATA1_OUT``` (Din23)
- ```GPB3``` → ```PIN_DATA2_OUT``` (Din24)
- ```GPB2``` → ```PIN_DATA3_OUT``` (Din25)
- ```GPB1``` → ```PIN_DATA4_OUT``` (Din26)
- ```GPB0``` → ```PIN_PROG_OUT``` (Din27)

Inputs (robot arm → Arduino). Used to determine the current arm pose.

- ```GPB7``` → ```PIN_ACK_IN``` (Dout27)
- ```GPA0``` → ```PIN_FAULT_IN``` (Dout21)
- ```GPA1``` → ```PIN_DATA0_IN``` (Dout22)
- ```GPA2``` → ```PIN_DATA1_IN``` (Dout23)
- ```GPA3``` → ```PIN_DATA2_IN``` (Dout24)
- ```GPA4``` → ```PIN_DATA3_IN``` (Dout25)

# 3. Finite State Machine
The program is organized with the help of a Finite State Machine (FSM). There are multiple commands to be sent to the robot, as well as multiple possible poses that the robot can take.

## List of commands (Arduino → robot arm)
The binary code is given by:

- Bit 0: ```PIN_DATA0_OUT``` (Din22)
- Bit 1: ```PIN_DATA1_OUT``` (Din23)
- Bit 2: ```PIN_DATA2_OUT``` (Din24)
- Bit 3: ```PIN_DATA3_OUT``` (Din25)
- Bit 4: ```PIN_DATA4_OUT``` (Din26)

The different commands are as follows:

- ```REST_TO_REST```→  0 (00000) -> Stay at rest
- ```REST_TO_PICK_COIN```→ 1  (00001) -> Go pick a coin
- ```PICK_COIN_TO_PICK_COIN_READY```→ 2  (00010) -> Go down at the pick coin location
- ```PICK_COIN_READY_TO_PIG_1```→ 3  (00011) -> Go to the first pig
- ```PICK_COIN_READY_TO_PIG_2```→ 4  (00100) -> Go to the second pig
- ```PICK_COIN_READY_TO_PIG_3```→ 5  (00101) -> Go to the third pig
- ```PICK_COIN_READY_TO_BIN```→ 6  (00110) -> Go to the bin
- ```PIG_1_TO_SPECIAL_1```→ 7  (00111) -> Go to the special move location 1 after pig 1
- ```PIG_2_TO_SPECIAL_2```→ 8  (01000) -> Go to the special move location 2 after pig 2
- ```PIG_3_TO_SPECIAL_3```→ 9  (01001) -> Go to the special move location 3 after pig 3
- ```BIN_TO_SPECIAL_BIN```→ 10 (01010) -> Go to the special move location (bin) after bin
- ```SPECIAL_1_TO_SPECIAL_MOVE_1```→ 11 (01011) -> Perform special move 1
- ```SPECIAL_2_TO_SPECIAL_MOVE_2```→ 12 (01100) -> Perform special move 2
- ```SPECIAL_3_TO_SPECIAL_MOVE_3```→ 13 (01101) -> Perform special move 3
- ```SPECIAL_BIN_TO_SPECIAL_MOVE_BIN```→ 14 (01110) -> Perform special move (bin)
- ```SPECIAL_MOVE_1_TO_REST```→ 15 (01111) -> Go back to resting position after special move 1
- ```SPECIAL_MOVE_2_TO_REST```→ 16 (10000) -> Go back to resting position after special move 2
- ```SPECIAL_MOVE_3_TO_REST```→ 17 (10001) -> Go back to resting position after special move 3
- ```SPECIAL_MOVE_BIN_TO_REST```→ 18 (10010) -> Go back to resting position after special move (bin)

## List of poses (robot arm → Arduino)
The binary code is given by:

- Bit 0: ```PIN_DATA0_IN``` (Dout22)
- Bit 1: ```PIN_DATA1_IN``` (Dout23)
- Bit 2: ```PIN_DATA2_IN``` (Dout24)
- Bit 3: ```PIN_DATA3_IN``` (Dout25)

The list of unique poses are as follows:

- ```RESTING_POSE```→ 1  (0001) -> Default pose
- ```PICK_COIN_POSE```→ 2  (0010) -> Just above the picking location
- ```PICK_COIN_READY_POSE```→ 3  (0011) -> After going down to pick the coin
- ```PIG_1_POSE```→ 4  (0100) -> Above the first pig
- ```PIG_2_POSE```→ 5  (0101) -> Above the second pig
- ```PIG_3_POSE```→ 6  (0110) -> Above the third pig
- ```BIN_POSE```→ 7  (0111) -> Above the bin
- ```SPECIAL_1_POSE```→ 8  (1000) -> To the 'special' location to go to after the first pig
- ```SPECIAL_2_POSE```→ 9  (1001) -> To the 'special' location to go to after the second pig
- ```SPECIAL_3_POSE```→ 10 (1010) -> To the 'special' location to go to after the third pig
- ```SPECIAL_BIN_POSE```→ 11 (1011) -> To the 'special' location to go to after the bin
- ```SPECIAL_MOVE_1_POSE```→ 12 (1100) -> Pose after the special move that was carried out at special location 1
- ```SPECIAL_MOVE_2_POSE```→ 13 (1101) -> Pose after the special move that was carried out at special location 2
- ```SPECIAL_MOVE_3_POSE```→ 14 (1110) -> Pose after the special move that was carried out at special location 3
- ```SPECIAL_MOVE_BIN_POSE```→ 15 (1111) -> Pose after the special move that was carried out at special location 'bin'

## Program flow

### List of states (for the FSM)

- ```Rest```: the default state of the robot when it's resting.
- ```Pick Coin```: when it's just above the picking location.
- ```Pick Coin Ready```: after coming down, when it's ready to pick up a coin.
- ```Close Fingers```: closes the fingers around the coin.
- ```Pig 1```: to go to the first pig (10, 20, 50 cents).
- ```Pig 2```: to go to the second pig (5 CHF).
- ```Pig 3```: to go to the third pig (1, 2 CHF).
- ```Bin```: to go to the bin (all other coins).
- ```Open Fingers```: to drop the coin in a pig/bin.
- ```Special 1```: to go to the special move location after the first pig.
- ```Special 2```: to go to the special move location after the second pig.
- ```Special 3```: to go to the special move location after the third pig.
- ```Special Bin```: to go to the special move location after the bin.
- ```Special Move 1```: Perform the special move after reaching the special location 1.
- ```Special Move 2```: Perform the special move after reaching the special location 2.
- ```Special Move 3```: Perform the special move after reaching the special location 3.
- ```Special Move Bin```: Perform the special move after reaching the special location (bin).
