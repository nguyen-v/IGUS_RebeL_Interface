#ifndef HAND_H
#define HAND_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <PololuMaestro.h>
#include "HandPoses.h"

#define MIN_TARGET 4000 // 1ms pulses
#define MAX_TARGET 8000 // 2ms pulses

#define MIN_TARGETS {4000, 4000, 4000, 4000, 4000}
#define MAX_TARGETS {8000, 8000, 8000, 8000, 8000}
#define MIN_VALUES {0.0, 0.0, 0.0, 0.0, 0.0}
#define MAX_VALUES {100.0, 100.0, 100.0, 100.0, 100.0}
#define SIGN {1, 1, 1, 1, -1}

class Hand {
public:
  Hand();
  void init();
  bool move_finished();
  void close();
  void pointing();
  void open();
  void natural();
  void grasping_pose();
  void grasp_coin();
  void drop_coin();
  void special_move_1();
  void special_move_2();
  void special_move_3();
  void special_move_bin();

  void update_state();
private:
  SoftwareSerial* maestro_serial;
  MicroMaestro* maestro;
  static void values_to_target(const float values[HAND_DOFS], int targets[HAND_DOFS]);
  void move(const float angles[][HAND_DOFS], size_t num_rows);
  static const float min_values[HAND_DOFS];
  static const float max_values[HAND_DOFS];
  static const uint16_t min_targets[HAND_DOFS];
  static const uint16_t max_targets[HAND_DOFS];
  static const int sign[HAND_DOFS];
  bool moving = false;
  int current_targets[HAND_DOFS];
  // how close we want to be to the target before marking the movement as completed
  static const uint16_t target_thr = 100;
};

#endif