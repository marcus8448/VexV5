#ifndef _AUTONOMOUS_HPP_
#define _AUTONOMOUS_HPP_

#include "robot.hpp"

void right_side_winpoint() {
    backwards(20.0, 80);
    arm_hook_close();
    arm_hook_open();
    arm_hook_close();
    turn_right(30);
    forwards(6.0);
    turn_right(30);
    forwards(20.0);
    arm_hook_open();
}

void left_side_winpoint() {
    backwards(20.0, 80);
    arm_hook_close();
    arm_hook_open();
    arm_hook_close();
    turn_right(30);
    forwards(6.0);
    turn_right(30);
    forwards(20.0);
    arm_hook_open();
}

void middle_right_goal() {
    backwards(34.0, 80);
    arm_hook_close();
    forwards(17.0, 80);
}

void middle_left_goal() {
    backwards(34.0, 80);
    arm_hook_close();
    forwards(17.0, 80);
}

#endif