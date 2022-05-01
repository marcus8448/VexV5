#include <iostream>
#include "devices.hpp"
#include "pros/rtos.hpp"

void reset_lift(void* param) {
    lift.move(20);
    delay(500);
    while (lift.get_efficiency() > 0.001) {
        delay(200);
    }
    lift.move(0);
    ((pros::Task)param).notify();
}

void reset_arm(void* param) {
    arm_hook.move(-40);
    arm_1.move(-25);
    arm_2.move(-25);
    delay(500);
    while (arm_hook.get_efficiency() > 0.001 || arm_1.get_efficiency() > 0.001 || arm_2.get_efficiency() > 0.0015) {
        delay(200);
    }
    arm_hook.move_relative(155.0, 30);
    arm_1.move(0);
    arm_2.move(0);
    while (abs(arm_hook.get_position() - arm_hook.get_target_position()) > 0.01) {
        delay(200);
    }
    arm_1.move(0);
    arm_2.move(0);
    ((pros::Task)param).notify();
}

void reset_positions() {
    int completed = 0;
    controller.set_text(0, 0, "Reset...");
    Task lift_reset(reset_lift, ((void*) pros::Task::current()), "Lift reset");
    Task arm_reset(reset_arm, ((void*) pros::Task::current()), "Arm reset");

    while (pros::Task::notify_take(false, TIMEOUT_MAX)) {
        if (++completed == 2) break;
    }

    controller.set_text(0, 0, "Done!");
    std::cout << "Done reset." << std::endl;
}
