#include "RobotArm.h"
#include <Arduino.h>
#include "PinManager.h"

uint16_t RobotArm::pose_state = RobotArm::Poses::RESTING_POSE;
bool RobotArm::flag_update_state = false;
bool RobotArm::flag_update_pose_state = false;

void RobotArm::send_command(uint16_t cmd) {
    Serial.println(F("Sending command to the arm..."));
    // Break the command into individual bits
    uint16_t data0 = (cmd >> 0) & 0x01;
    uint16_t data1 = (cmd >> 1) & 0x01;
    uint16_t data2 = (cmd >> 2) & 0x01;
    uint16_t data3 = (cmd >> 3) & 0x01;
    uint16_t data4 = (cmd >> 4) & 0x01;

    // Write the bits to the output pins
    PinManager::mcp.digitalWrite(PinManager::PIN_DATA0_OUT, data0);
    PinManager::mcp.digitalWrite(PinManager::PIN_DATA1_OUT, data1);
    PinManager::mcp.digitalWrite(PinManager::PIN_DATA2_OUT, data2);
    PinManager::mcp.digitalWrite(PinManager::PIN_DATA3_OUT, data3);
    PinManager::mcp.digitalWrite(PinManager::PIN_DATA4_OUT, data4);

    // Trigger the acknowledgment signal
    delay(200); // Command propagation delay
    PinManager::mcp.digitalWrite(PinManager::PIN_ACK_OUT, HIGH);
    print_command(cmd);
}

void RobotArm::enable() {
    PinManager::mcp.digitalWrite(PinManager::PIN_ENABLE_OUT, HIGH);
    Serial.println(F("Enabling the arm..."));
}

void RobotArm::disable() {
    PinManager::mcp.digitalWrite(PinManager::PIN_ENABLE_OUT, LOW);
    Serial.println(F("Fault signal detected, disabling the arm..."));
}

void RobotArm::irq_update_state() {
  digitalWrite(LED_BUILTIN, HIGH);
  flag_update_state = true;
}

void RobotArm::update_state() {
  if (flag_update_state) {
    flag_update_state = false;
    Serial.println("Fault flag set.");
    disable();
  }
  if (flag_update_pose_state) {
    Serial.println("Update pose state flag set.");
    flag_update_pose_state = false;
    uint16_t data0 = !PinManager::mcp.digitalRead(PinManager::PIN_DATA0_IN);
    uint16_t data1 = !PinManager::mcp.digitalRead(PinManager::PIN_DATA1_IN);
    uint16_t data2 = !PinManager::mcp.digitalRead(PinManager::PIN_DATA2_IN);
    uint16_t data3 = !PinManager::mcp.digitalRead(PinManager::PIN_DATA3_IN);
    pose_state = (data3 << 3) | (data2 << 2) | (data1 << 1) | (data0 << 0);
    PinManager::mcp.digitalWrite(PinManager::PIN_ACK_OUT, LOW);
    print_pose(pose_state);
  }
}

void RobotArm::print_pose(uint16_t id) {
    switch (id) {
        case RESTING_POSE:
            Serial.println(F("Resting Pose"));
            break;
        case PICK_COIN_POSE:
            Serial.println(F("Pick Coin Pose"));
            break;
        case PICK_COIN_READY_POSE:
            Serial.println(F("Pick Coin Ready Pose"));
            break;
        case PIG_1_POSE:
            Serial.println(F("Pig 1 Pose"));
            break;
        case PIG_2_POSE:
            Serial.println(F("Pig 2 Pose"));
            break;
        case PIG_3_POSE:
            Serial.println(F("Pig 3 Pose"));
            break;
        case BIN_POSE:
            Serial.println(F("Bin Pose"));
            break;
        case SPECIAL_1_POSE:
            Serial.println(F("Special 1 Pose"));
            break;
        case SPECIAL_2_POSE:
            Serial.println(F("Special 2 Pose"));
            break;
        case SPECIAL_3_POSE:
            Serial.println(F("Special 3 Pose"));
            break;
        case SPECIAL_BIN_POSE:
            Serial.println(F("Special Bin Pose"));
            break;
        case SPECIAL_MOVE_1_POSE:
            Serial.println(F("Special Move 1 Pose"));
            break;
        case SPECIAL_MOVE_2_POSE:
            Serial.println(F("Special Move 2 Pose"));
            break;
        case SPECIAL_MOVE_3_POSE:
            Serial.println(F("Special Move 3 Pose"));
            break;
        case SPECIAL_MOVE_BIN_POSE:
            Serial.println(F("Special Move Bin Pose"));
            break;
        default:
            Serial.println(F("Unknown Pose"));
            break;
    }
}

void RobotArm::print_command(uint16_t id) {
    switch (id) {
        case REST_TO_REST:
            Serial.println(F("Rest to Rest"));
            break;
        case REST_TO_PICK_COIN:
            Serial.println(F("Rest to Pick Coin"));
            break;
        case PICK_COIN_TO_PICK_COIN_READY:
            Serial.println(F("Pick Coin to Pick Coin Ready"));
            break;
        case PICK_COIN_READY_TO_PIG_1:
            Serial.println(F("Pick Coin Ready to Pig 1"));
            break;
        case PICK_COIN_READY_TO_PIG_2:
            Serial.println(F("Pick Coin Ready to Pig 2"));
            break;
        case PICK_COIN_READY_TO_PIG_3:
            Serial.println(F("Pick Coin Ready to Pig 3"));
            break;
        case PICK_COIN_READY_TO_BIN:
            Serial.println(F("Pick Coin Ready to Bin"));
            break;
        case PIG_1_TO_SPECIAL_1:
            Serial.println(F("Pig 1 to Special 1"));
            break;
        case PIG_2_TO_SPECIAL_2:
            Serial.println(F("Pig 2 to Special 2"));
            break;
        case PIG_3_TO_SPECIAL_3:
            Serial.println(F("Pig 3 to Special 3"));
            break;
        case BIN_TO_SPECIAL_BIN:
            Serial.println(F("Bin to Special Bin"));
            break;
        case SPECIAL_1_TO_SPECIAL_MOVE_1:
            Serial.println(F("Special 1 to Special Move 1"));
            break;
        case SPECIAL_2_TO_SPECIAL_MOVE_2:
            Serial.println(F("Special 2 to Special Move 2"));
            break;
        case SPECIAL_3_TO_SPECIAL_MOVE_3:
            Serial.println(F("Special 3 to Special Move 3"));
            break;
        case SPECIAL_BIN_TO_SPECIAL_MOVE_BIN:
            Serial.println(F("Special Bin to Special Move Bin"));
            break;
        case SPECIAL_MOVE_1_TO_REST:
            Serial.println(F("Special Move 1 to Rest"));
            break;
        case SPECIAL_MOVE_2_TO_REST:
            Serial.println(F("Special Move 2 to Rest"));
            break;
        case SPECIAL_MOVE_3_TO_REST:
            Serial.println(F("Special Move 3 to Rest"));
            break;
        case SPECIAL_MOVE_BIN_TO_REST:
            Serial.println(F("Special Move Bin to Rest"));
            break;
        default:
            Serial.println(F("Unknown Command"));
            break;
    }
}

void RobotArm::irq_update_pose_state() {
  digitalWrite(LED_BUILTIN, HIGH);
  flag_update_pose_state = true;
}

uint16_t RobotArm::get_pose() {
  return pose_state;
}