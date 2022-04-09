#ifndef _AUTONOMOUS_HPP_
#define _AUTONOMOUS_HPP_

#include "robot.hpp"

void right_side_winpoint() {
    backwards(22.0, 80);
    arm_hook_close();
    delay(1000);
    forwards(12.0);
    turn_right(90);
}

void left_side_winpoint() {
    backwards(22.0, 80);
    arm_hook_close();
    delay(1000);
    forwards(12.0);
    turn_right(90);
}

void middle_right_goal() {
    backwards(34.0, 80);
}

void middle_left_goal() {
    backwards(34.0, 80);
}

#endif