#include "pros/rtos.hpp"
#include "robot.hpp"
#include <iostream>

void reset_positions(Robot* robot) {
    robot->controller->set_line(0, 0, "Reset...");

    for (unsigned char i = 0; i < 2; i += pros::Task::notify_take(true, 0xffffffffUL)) {
    }

    std::cout << "Done reset." << std::endl;
    robot->reset();
    robot->controller->set_line(0, 0, "Done!");
}
