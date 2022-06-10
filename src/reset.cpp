#include <iostream>
#include "pros/rtos.hpp"
#include "vexv5/robot.hpp"

void reset_positions(Robot robot) {
    unsigned int completed = 0;
    robot.controller.set_line(0, 0, "Reset...");
    
    while (completed < 2) {
        completed += pros::Task::notify_take(true, TIMEOUT_MAX);
    }

    std::cout << "Done reset." << std::endl;
    robot.reset();
    while (true) {
        robot.controller.set_line(0, 0, "Done!");
        pros::delay(1000);
    }
}
