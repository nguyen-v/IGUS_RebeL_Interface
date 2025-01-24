#include "Hand.h"
#include <Arduino.h>
#include "PinManager.h"

static const float Hand::min_values[HAND_DOFS] = MIN_VALUES;
static const float Hand::max_values[HAND_DOFS] = MAX_VALUES;
static const uint16_t Hand::min_targets[HAND_DOFS] = MIN_TARGETS;
static const uint16_t Hand::max_targets[HAND_DOFS] = MAX_TARGETS;
static const int Hand::sign[HAND_DOFS] = SIGN;

Hand::Hand() {
  maestro_serial = new SoftwareSerial(PinManager::PIN_HAND_RX, PinManager::PIN_HAND_TX);
  maestro = new MicroMaestro(*maestro_serial);

}

void Hand::init() {
  maestro_serial->begin(115200);
  for (uint8_t i = 0; i < HAND_DOFS; ++i) {
    maestro->setSpeed(i, 0);
    maestro->setAcceleration(i, 0);
  }
  natural();
}

void Hand::update_state() {
  Serial.println(F("Updating hand state..."));
  bool completed = true;
  for (uint8_t i = 0; i < HAND_DOFS; ++i)
  {
    int position = maestro->getPosition(i+1);
    if (current_targets[i] == -1)
      continue;
    if (abs(position - current_targets[i]) > target_thr) {
      completed = false;
      break;
    }
  }
  if (completed) {
    moving = false;
  } else {
    moving = true;
  }
}

void Hand::move(const float values[][HAND_DOFS], size_t num_rows) {
    int targets[HAND_DOFS];
    moving = true;

    // First we save the last target to reach. This is used to check for movement completion
    float last_row[HAND_DOFS];
    for (uint8_t j = 0; j < HAND_DOFS; ++j) {
      last_row[j] = pgm_read_float(&(values[num_rows-1][j]));
    }
    values_to_target(last_row, current_targets);

    // Then we move each finger
    for (size_t i = 0; i < num_rows; ++i) {
        float row[HAND_DOFS];
        for (uint8_t j = 0; j < HAND_DOFS; ++j) {
            row[j] = pgm_read_float(&(values[i][j]));
        }
        
        values_to_target(row, targets);

        for (uint8_t j = 0; j < HAND_DOFS; ++j) {
            if (targets[j] != -1) {  // Ignore DOFs where the angle was set as -1
                maestro->setSpeed(j + 1, 15);
                maestro->setAcceleration(j + 1, 2);
                maestro->setTarget(j + 1, targets[j]);
                delay(50);
            }
        }
        // This delay is blocking; consider a non-blocking alternative
        delay(100);
    }
}

void Hand::values_to_target(const float values[HAND_DOFS], uint16_t targets[HAND_DOFS]) {
    for (size_t i = 0; i < HAND_DOFS; ++i) {
        if (values[i] == -1) {
          targets[i] = -1;
        }
        else {
          if (sign[i] == 1) {
            targets[i] = min_targets[i] + (max_targets[i] - min_targets[i]) * 
                        (values[i] - min_values[i]) / (max_values[i] - min_values[i]);
          } else if (sign[i] == -1) {
            targets[i] = max_targets[i] - (max_targets[i] - min_targets[i]) * 
                        (values[i] - min_values[i]) / (max_values[i] - min_values[i]);            
          }
        }
    }
}


bool Hand::move_finished() {
  return !moving;
}

void Hand::close() {
  Serial.println(F("Closing the hand..."));
  move(CLOSE_VALUES, 1);
}

void Hand::pointing() {
  Serial.println(F("Pointing..."));
  move(POINTING_VALUES, 1);
}

void Hand::open() {
  Serial.println(F("Opening the hand..."));
  move(OPEN_VALUES, 1);
}

void Hand::natural() {
  Serial.println(F("Hand at natural pose..."));
  move(NATURAL_VALUES, 1);
}

void Hand::grasping_pose() {
  Serial.println(F("Hand at grasping pose..."));
  move(GRASPING_VALUES, 1);
}

void Hand::grasp_coin() {
  Serial.println(F("Hand at grasping coin pose..."));
  move(GRASPING_COIN_VALUES, 1);
}

void Hand::drop_coin() {
  Serial.println(F("Dropping coin..."));
  move(DROP_COIN_VALUES, 1);
}

void Hand::special_move_1() {
  move(WAVE_VALUES, 15);
}

void Hand::special_move_2() {

}

void Hand::special_move_3() {

}

void Hand::special_move_bin() {

}
