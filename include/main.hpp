#ifndef VEXV5_MAIN_HPP
#define VEXV5_MAIN_HPP
#include "robot/robot.hpp"

robot::Robot *get_or_create_robot();

extern "C" {
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
}
#endif // VEXV5_MAIN_HPP
