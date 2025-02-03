#ifndef HAND_POSES_H
#define HAND_POSES_H

#define HAND_DOFS 5 // Number of Degrees of Freedoms in the hands

// Fingers
// 0 means extended finger, 100 is closed finger
//     __,------,         5
// ___/   __/______ __    4 (internal rotation). Higher means grip, lower means open palm
//           _________)   1
//           _________)   2
// __________________)    3

const float NATURAL_VALUES[][HAND_DOFS] PROGMEM = {
  {10, 20, 30, 50, 50},
};

const float OPEN_VALUES[][HAND_DOFS] PROGMEM = {
  {10, 10, 10, 10, 50},
};

const float CLOSE_VALUES[][HAND_DOFS] PROGMEM = {
  {60, 60, 60, 10, 70},
};

const float POINTING_VALUES[][HAND_DOFS] PROGMEM = {
  {10, 90, 90, 100, 90},
};

const float SPECIAL_1_VALUES[][HAND_DOFS] PROGMEM = {
  {10, 20, 30, 50, 50},
  {20, 30, 40, 50, 50},
  {30, 40, 50, 50, 50},
  {40, 50, 60, 50, 50},
  {50, 60, 70, 50, 50},
  {60, 70, 80, 50, 50},
  {70, 80, 90, 50, 50},
  {80, 70, 80, 50, 50},
  {70, 60, 70, 50, 50},
  {60, 50, 60, 50, 50},
  {50, 40, 50, 50, 50},
  {40, 30, 40, 50, 50},
  {30, 20, 30, 50, 50},
  {20, 10, 20, 50, 50},
  {10, 20, 30, 50, 50},
};

const float SPECIAL_2_VALUES[][HAND_DOFS] PROGMEM = {
  {10, 20, 30, 50, 50},
  {20, 30, 40, 50, 50},
  {30, 40, 50, 50, 50},
  {40, 50, 60, 50, 50},
  {50, 60, 70, 50, 50},
  {60, 70, 80, 50, 50},
  {70, 80, 90, 50, 50},
  {80, 70, 80, 50, 50},
  {70, 60, 70, 50, 50},
  {60, 50, 60, 50, 50},
  {50, 40, 50, 50, 50},
  {40, 30, 40, 50, 50},
  {30, 20, 30, 50, 50},
  {20, 10, 20, 50, 50},
  {10, 20, 30, 50, 50},
};

const float SPECIAL_3_VALUES[][HAND_DOFS] PROGMEM = {
  {10, 20, 30, 50, 50},
  {20, 30, 40, 50, 50},
  {30, 40, 50, 50, 50},
  {40, 50, 60, 50, 50},
  {50, 60, 70, 50, 50},
  {60, 70, 80, 50, 50},
  {70, 80, 90, 50, 50},
  {80, 70, 80, 50, 50},
  {70, 60, 70, 50, 50},
  {60, 50, 60, 50, 50},
  {50, 40, 50, 50, 50},
  {40, 30, 40, 50, 50},
  {30, 20, 30, 50, 50},
  {20, 10, 20, 50, 50},
  {10, 20, 30, 50, 50},
};

const float SPECIAL_BIN_VALUES[][HAND_DOFS] PROGMEM = {
  {10, 20, 30, 50, 50},
  {20, 30, 40, 50, 50},
  {30, 40, 50, 50, 50},
  {40, 50, 60, 50, 50},
  {50, 60, 70, 50, 50},
  {60, 70, 80, 50, 50},
  {70, 80, 90, 50, 50},
  {80, 70, 80, 50, 50},
  {70, 60, 70, 50, 50},
  {60, 50, 60, 50, 50},
  {50, 40, 50, 50, 50},
  {40, 30, 40, 50, 50},
  {30, 20, 30, 50, 50},
  {20, 10, 20, 50, 50},
  {10, 20, 30, 50, 50},
};

const float GRASPING_VALUES[][HAND_DOFS] PROGMEM = {
  {50, 100, 100, 100, 50},
};

const float GRASPING_COIN_VALUES[][HAND_DOFS] PROGMEM = {
  {70, -1, -1, 100, 67.5},
};

const float DROP_COIN_VALUES[][HAND_DOFS] PROGMEM = {
  {50, -1, -1, 100, 50},
};

#endif