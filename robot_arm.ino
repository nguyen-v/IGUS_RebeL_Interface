#include <StateMachine.h>
#include "RobotArm.h"
#include "PinManager.h"
#include "Hand.h"

#define ADD_TRANSITIONS

// Finite State Machine ===========================================

StateMachine fsm = StateMachine();
PinManager pin_manager = PinManager();
Hand hand = Hand();
RobotArm arm = RobotArm();

State* rest_state = fsm.addState(&rest_cb);
State* pick_coin_state = fsm.addState(&pick_coin_cb);
State* pick_coin_ready_state = fsm.addState(&pick_coin_ready_cb);
State* close_fingers_state = fsm.addState(&close_fingers_cb);
State* pig_1_state = fsm.addState(&pig_1_cb);
State* pig_2_state = fsm.addState(&pig_2_cb);
State* pig_3_state = fsm.addState(&pig_3_cb);
State* bin_state = fsm.addState(&bin_cb);
State* open_fingers_state = fsm.addState(&open_fingers_cb);
State* special_1_state = fsm.addState(&special_1_cb);
State* special_2_state = fsm.addState(&special_2_cb);
State* special_3_state = fsm.addState(&special_3_cb);
State* special_bin_state = fsm.addState(&special_bin_cb);
State* special_move_1_state = fsm.addState(&special_move_1_cb);
State* special_move_2_state = fsm.addState(&special_move_2_cb);
State* special_move_3_state = fsm.addState(&special_move_3_cb);
State* special_move_bin_state = fsm.addState(&special_move_bin_cb);

using Poses = RobotArm::Poses;

enum Coins {
  NO_COINS = 0,  // No coin
  INVALID_COIN,  // Invalid coin
  CHF_005,       // 5 cents
  CHF_010,       // 10 cents
  CHF_020,       // 20 cents
  CHF_050,       // 50 cents
  CHF_100,       // 1 CHF
  CHF_200,       // 2 CHF
  CHF_500        // 5 CHF
};

// Coin type
uint8_t coin_type = INVALID_COIN;

// Force next state (for debugging)
bool next = false;
bool next1 = false;
bool next2 = false;
bool next3 = false;
bool nextbin = false;

void reset_next() {
  next = false;
  next1 = false;
  next2 = false;
  next3 = false;
  nextbin = false;
}

// ================================================================

void setup() {

  Serial.begin(115200);

  pin_manager.setup_pins();

  // Turn off LEDs for now
  digitalWrite(PinManager::PIN_LED_R, LOW);
  digitalWrite(PinManager::PIN_LED_G, LOW);

  hand.init();

  // Interrupts are active LOW (because of 100k pull-up)
  attachInterrupt(digitalPinToInterrupt(PinManager::PIN_INTB_ACK_IN), RobotArm::update_pose_state, FALLING); // for synchronisation
  attachInterrupt(digitalPinToInterrupt(PinManager::PIN_INTA_FAULT_IN), RobotArm::update_state, CHANGE);

  // Enable the robot arm
  arm.enable();

  // Add the FSM transitions
  #ifdef ADD_TRANSITIONS
    rest_state->addTransition(&pick_coin_tran, pick_coin_state);

    pick_coin_state->addTransition(&pick_coin_ready_tran, pick_coin_ready_state);
    pick_coin_ready_state->addTransition(&close_fingers_tran, close_fingers_state);

    close_fingers_state->addTransition(&pig_1_tran, pig_1_state);
    close_fingers_state->addTransition(&pig_2_tran, pig_2_state);
    close_fingers_state->addTransition(&pig_3_tran, pig_3_state);
    close_fingers_state->addTransition(&bin_tran, bin_state);

    pig_1_state->addTransition(&open_fingers_tran, open_fingers_state);
    pig_2_state->addTransition(&open_fingers_tran, open_fingers_state);
    pig_3_state->addTransition(&open_fingers_tran, open_fingers_state);
    bin_state->addTransition(&open_fingers_tran, open_fingers_state);

    open_fingers_state->addTransition(&special_1_tran, special_1_state);
    open_fingers_state->addTransition(&special_2_tran, special_1_state);
    open_fingers_state->addTransition(&special_3_tran, special_1_state);
    open_fingers_state->addTransition(&special_bin_tran, special_bin_state);

    special_1_state->addTransition(&special_move_1_tran, special_move_1_state);
    special_2_state->addTransition(&special_move_2_tran, special_move_2_state);
    special_3_state->addTransition(&special_move_3_tran, special_move_3_state);
    special_bin_state->addTransition(&special_move_bin_tran, special_move_bin_state);
    special_move_1_state->addTransition(&rest_tran, rest_state);
    special_move_2_state->addTransition(&rest_tran, rest_state);
    special_move_3_state->addTransition(&rest_tran, rest_state);
    special_move_bin_state->addTransition(&rest_tran, rest_state);
  #endif

  // Open the solenoid by default after everything has been initialized
  digitalWrite(PinManager::PIN_SOLENOID, LOW);
  // Turn on the green LED
  digitalWrite(PinManager::PIN_LED_G, HIGH);
}

void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read(); // Read a single character from the serial buffer
    Serial.print(F("Input: "));
    Serial.println(input);

    // Check the input character and take appropriate action
    if (input == 'n') {
      next = true;
      Serial.println(F("Going to next state"));
    } else if (input == '1') {
      next1 = true;
      Serial.println(F("Going to next state 1"));
    } else if (input == '2') {
      next2 = true;
      Serial.println(F("Going to next state 2"));
    } else if (input == '3') {
      next3 = true;
      Serial.println(F("Going to next state 3"));
    } else if (input == 'b') {
      nextbin = true;
      Serial.println(F("Going to next state bin"));
    }
  }

  hand.update_state();
  // check for FSM state changes at regular intervals
  fsm.run();
  delay(500);

}

// Transitions ================================================================

bool pick_coin_tran() {
  if ((RobotArm::get_pose() == Poses::RESTING_POSE) || next) {
    reset_next();
    return true;
  }
  return false;
}

bool pick_coin_ready_tran() {
  if ((RobotArm::get_pose() == Poses::PICK_COIN_POSE) || next) { // also check for coin here
    reset_next();
    return true;
  }
  return false;
}

bool close_fingers_tran() {
  if ((RobotArm::get_pose() == Poses::PICK_COIN_READY_POSE) || next) {
    reset_next();
    return true;
  }
  return false;
}

// For pig_x_tran and bin_tran, maybe wait for a flag for fingers to finish closing?

bool pig_1_tran() {
  if ((RobotArm::get_pose() == Poses::PICK_COIN_READY_POSE) && 
      (coin_type == CHF_005) || (coin_type == CHF_010) ||
      (coin_type == CHF_020) || (coin_type == CHF_050)
      && (hand.move_finished()) || next1) {
    reset_next();
    return true;
  }
  return false;
}

bool pig_2_tran() {
  if ((RobotArm::get_pose() == Poses::PICK_COIN_READY_POSE) &&
      (coin_type == CHF_100) || (coin_type == CHF_200)
      && (hand.move_finished()) || next2) {
    reset_next();
    return true;
  }
  return false;
}

bool pig_3_tran() {
  if ((RobotArm::get_pose() == Poses::PICK_COIN_READY_POSE) &&
      (coin_type == CHF_500)
      && (hand.move_finished()) || next3) {
    reset_next();
    return true;
  }
  return false;
}

bool bin_tran() {
  if ((RobotArm::get_pose() == Poses::PICK_COIN_READY_POSE) &&
    (coin_type == INVALID_COIN)
    && (hand.move_finished()) || nextbin) {
    reset_next();
    return true;
  }
  return false;
}

bool open_fingers_tran() {
  if ((RobotArm::get_pose() == Poses::PIG_1_POSE) || (RobotArm::get_pose() == Poses::PIG_2_POSE) ||
      (RobotArm::get_pose() == Poses::PIG_3_POSE) || (RobotArm::get_pose() == Poses::BIN_POSE) || next) {
    reset_next();
    return true;
  }
  return false;
}

// For the special_x_tran, maybe wait for a flag for fingers to finish opening?

bool special_1_tran() {
  if ((RobotArm::get_pose() == Poses::PIG_1_POSE)
  && (hand.move_finished()) || next1) {
    reset_next();
    return true;
  }
  return false;
}

bool special_2_tran() {
  if ((RobotArm::get_pose() == Poses::PIG_2_POSE)
  && (hand.move_finished()) || next2) {
    reset_next();
    return true;
  }
  return false;
}

bool special_3_tran() {
  if ((RobotArm::get_pose() == Poses::PIG_3_POSE)
  && (hand.move_finished()) || next3) {
    reset_next();
    return true;
  }
  return false;
}

bool special_bin_tran() {
  if ((RobotArm::get_pose() == Poses::BIN_POSE)
  && (hand.move_finished()) || nextbin) {
    reset_next();
    return true;
  }
  return false;
}

bool special_move_1_tran() {
  if ((RobotArm::get_pose() == Poses::SPECIAL_1_POSE) || next1) {
    reset_next();
    return true;
  }
  return false;
}

bool special_move_2_tran() {
  if (RobotArm::get_pose() == Poses::SPECIAL_2_POSE || next2) {
    reset_next();
    return true;
  }
  return false;
}

bool special_move_3_tran() {
  if (RobotArm::get_pose() == Poses::SPECIAL_3_POSE || next3) {
    reset_next();
    return true;
  }
  return false;
}

bool special_move_bin_tran() {
  if (RobotArm::get_pose() == Poses::SPECIAL_BIN_POSE || nextbin) {
    reset_next();
    return true;
  }
  return false;
}

bool rest_tran() {
  if ((RobotArm::get_pose() == Poses::SPECIAL_MOVE_1_POSE) || (RobotArm::get_pose() == Poses::SPECIAL_MOVE_2_POSE) ||
      (RobotArm::get_pose() == Poses::SPECIAL_MOVE_3_POSE) || (RobotArm::get_pose() == Poses::SPECIAL_MOVE_BIN_POSE) || next) {
    reset_next();
    return true;
  }
  return false;
}

// Callback functions ===============================================================

void rest_cb() {
  Serial.println("Resting state callback called");
  switch (RobotArm::get_pose()) {
    case Poses::SPECIAL_MOVE_1_POSE:
      arm.send_command(RobotArm::Commands::SPECIAL_MOVE_1_TO_REST);
      break;
    case Poses::SPECIAL_MOVE_2_POSE:
      arm.send_command(RobotArm::Commands::SPECIAL_MOVE_2_TO_REST);
      break;
    case Poses::SPECIAL_MOVE_3_POSE:
      arm.send_command(RobotArm::Commands::SPECIAL_MOVE_3_TO_REST);
      break;
    case Poses::SPECIAL_MOVE_BIN_POSE:
      arm.send_command(RobotArm::Commands::SPECIAL_MOVE_BIN_TO_REST);
      break;
    case Poses::RESTING_POSE:
      // arm.send_command(RobotArm::Commands::REST_TO_REST);
      // Open the solenoid
      digitalWrite(PinManager::PIN_SOLENOID, LOW);
      // Set LEDs to GREEN
      digitalWrite(PinManager::PIN_LED_G, HIGH);
      digitalWrite(PinManager::PIN_LED_R, LOW);
      hand.close();
      break;
    default:
      break;
  }
}

void pick_coin_cb() {
  Serial.println("Pick Coin state callback called");
  arm.send_command(RobotArm::Commands::REST_TO_PICK_COIN);
  hand.natural(); // natural hand pose
  // Close the solenoid
  digitalWrite(PinManager::PIN_SOLENOID, HIGH);
  // Set LEDs to RED
  digitalWrite(PinManager::PIN_LED_G, LOW);
  digitalWrite(PinManager::PIN_LED_R, HIGH);
}

void pick_coin_ready_cb() {
  Serial.println("Pick Coin Ready state callback called");
  arm.send_command(RobotArm::Commands::PICK_COIN_TO_PICK_COIN_READY);
  hand.grasping_pose();
}

void close_fingers_cb() {
  Serial.println("Close Fingers state callback called");
  hand.grasp_coin();
}

void pig_1_cb() {
  Serial.println("Pig 1 state callback called");
  arm.send_command(RobotArm::Commands::PICK_COIN_READY_TO_PIG_1);
}

void pig_2_cb() {
  Serial.println("Pig 2 state callback called");
  arm.send_command(RobotArm::Commands::PICK_COIN_READY_TO_PIG_2);
}

void pig_3_cb() {
  Serial.println("Pig 3 state callback called");
  arm.send_command(RobotArm::Commands::PICK_COIN_READY_TO_PIG_3);
}

void bin_cb() {
  Serial.println("Bin state callback called");
  arm.send_command(RobotArm::Commands::PICK_COIN_READY_TO_BIN);
}

void open_fingers_cb() {
  Serial.println("Open Fingers state callback called");
  hand.drop_coin();
}

void special_1_cb() {
  Serial.println("Special 1 state callback called");
  arm.send_command(RobotArm::Commands::PIG_1_TO_SPECIAL_1);
  hand.natural();
}

void special_2_cb() {
  Serial.println("Special 2 state callback called");
  arm.send_command(RobotArm::Commands::PIG_2_TO_SPECIAL_2);
  hand.natural();
}

void special_3_cb() {
  Serial.println("Special 3 state callback called");
  arm.send_command(RobotArm::Commands::PIG_3_TO_SPECIAL_3);
  hand.natural();
}

void special_bin_cb() {
  Serial.println("Special Bin state callback called");
  arm.send_command(RobotArm::Commands::BIN_TO_SPECIAL_BIN);
  hand.natural();
}

void special_move_1_cb() {
  Serial.println("Special Move 1 state callback called");
  arm.send_command(RobotArm::Commands::SPECIAL_1_TO_SPECIAL_MOVE_1);
  hand.special_move_1();
}

void special_move_2_cb() {
  Serial.println("Special Move 2 state callback called");
  arm.send_command(RobotArm::Commands::SPECIAL_2_TO_SPECIAL_MOVE_2);
  hand.special_move_2();
}

void special_move_3_cb() {
  Serial.println("Special Move 3 state callback called");
  arm.send_command(RobotArm::Commands::SPECIAL_3_TO_SPECIAL_MOVE_3);
  hand.special_move_3();
}

void special_move_bin_cb() {
  Serial.println("Special Move Bin state callback called");
  arm.send_command(RobotArm::Commands::SPECIAL_BIN_TO_SPECIAL_MOVE_BIN);
  hand.special_move_bin();
}