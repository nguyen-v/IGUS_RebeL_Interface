#ifndef ROBOT_ARM_H
#define ROBOT_ARM_H

#include <Arduino.h>

class RobotArm {
public:
    // Sends a command to the robotic arm
    static void send_command(uint16_t cmd);

    static void enable_arm();

    static void disable_arm();

    static void update_state();

    static void update_pose_state();

    // Enum for all possible arm commands
    enum Commands {
        REST_TO_REST = 0,                 // 0  (00000) -> Stay at rest
        REST_TO_PICK_COIN,                // 1  (00001) -> Go pick a coin
        PICK_COIN_TO_PICK_COIN_READY,     // 2  (00010) -> Go down at the pick coin location
        PICK_COIN_READY_TO_PIG_1,         // 3  (00011) -> Go to the first pig
        PICK_COIN_READY_TO_PIG_2,         // 4  (00100) -> Go to the second pig
        PICK_COIN_READY_TO_PIG_3,         // 5  (00101) -> Go to the third pig
        PICK_COIN_READY_TO_BIN,           // 6  (00110) -> Go to the bin
        PIG_1_TO_SPECIAL_1,               // 7  (00111) -> Go to the special move location 1 after pig 1
        PIG_2_TO_SPECIAL_2,               // 8  (01000) -> Go to the special move location 2 after pig 2
        PIG_3_TO_SPECIAL_3,               // 9  (01001) -> Go to the special move location 3 after pig 3
        BIN_TO_SPECIAL_BIN,               // 10 (01010) -> Go to the special move location (bin) after bin
        SPECIAL_1_TO_SPECIAL_MOVE_1,      // 11 (01011) -> Perform special move 1
        SPECIAL_2_TO_SPECIAL_MOVE_2,      // 12 (01100) -> Perform special move 2
        SPECIAL_3_TO_SPECIAL_MOVE_3,      // 13 (01101) -> Perform special move 3
        SPECIAL_BIN_TO_SPECIAL_MOVE_BIN,  // 14 (01110) -> Perform special move (bin)
        SPECIAL_MOVE_1_TO_REST,           // 15 (01111) -> Go back to resting position after special move 1
        SPECIAL_MOVE_2_TO_REST,           // 16 (10000) -> Go back to resting position after special move 2
        SPECIAL_MOVE_3_TO_REST,           // 17 (10001) -> Go back to resting position after special move 3
        SPECIAL_MOVE_BIN_TO_REST          // 18 (10010) -> Go back to resting position after special move (bin)
    };

    // Enum for all possible poses
    enum Poses {
        RESTING_POSE = 1,      // 1  (0001) -> Default pose
        PICK_COIN_POSE,        // 2  (0010) -> Just above the picking location
        PICK_COIN_READY_POSE,  // 3  (0011) -> After going down to pick the coin
        PIG_1_POSE,            // 4  (0100) -> Above the first pig
        PIG_2_POSE,            // 5  (0101) -> Above the second pig
        PIG_3_POSE,            // 6  (0110) -> Above the third pig
        BIN_POSE,              // 7  (0111) -> Above the bin
        SPECIAL_1_POSE,        // 8  (1000) -> To the 'special' location to go to after the first pig
        SPECIAL_2_POSE,        // 9  (1001) -> To the 'special' location to go to after the second pig
        SPECIAL_3_POSE,        // 10 (1010) -> To the 'special' location to go to after the third pig
        SPECIAL_BIN_POSE,      // 11 (1011) -> To the 'special' location to go to after the bin
        SPECIAL_MOVE_1_POSE,   // 12 (1100) -> Pose after the special move that was carried out at special location 1
        SPECIAL_MOVE_2_POSE,   // 13 (1101) -> Pose after the special move that was carried out at special location 2
        SPECIAL_MOVE_3_POSE,   // 14 (1110) -> Pose after the special move that was carried out at special location 3
        SPECIAL_MOVE_BIN_POSE  // 15 (1111) -> Pose after the special move that was carried out at special location 'bin'
    };

    static void print_pose(uint16_t id);
    static void print_command(uint16_t id);
    static uint16_t pose_state;
};

#endif
