#include <iostream>
#include "devices.hpp"
#include "pros/rtos.hpp"

void reset_all_motors() {
    lift.move(0);
    lift.tare_position();
    arm_1.move(0);
    arm_1.tare_position();
    arm_2.move(0);
    arm_2.tare_position();
    arm_hook.move(0);
    arm_hook.tare_position();
    motor_rf.move(0);
    motor_rf.tare_position();
    motor_rb.move(0);
    motor_rb.tare_position();
    motor_lf.move(0);
    motor_lf.tare_position();
    motor_lb.move(0);
    motor_lb.tare_position();
}

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
    while (abs(arm_hook.get_position() - arm_hook.get_target_position()) > 1.75) {
        delay(200);
    }
    arm_1.move_relative(105.0, 50);
    arm_2.move_relative(105.0, 50);
    while (abs(arm_1.get_position() - arm_1.get_target_position()) > 1.75 || abs(arm_2.get_position() - arm_2.get_target_position()) > 1.75) {
        delay(200);
    }
    arm_1.move(0);
    arm_2.move(0);
    arm_hook.move(0);
    ((pros::Task)param).notify();
}

void reset_positions() {
    unsigned int completed = 0;
    controller.set_text(0, 0, "Reset...");
    Task lift_reset(reset_lift, ((void*) pros::Task::current()), "Lift reset");
    Task arm_reset(reset_arm, ((void*) pros::Task::current()), "Arm reset");
    
    while (completed < 2) {
        completed += pros::Task::notify_take(true, TIMEOUT_MAX);
    }

    std::cout << "Done reset." << std::endl;
    reset_all_motors();
    while (true) {
        controller.set_text(0, 0, "Done!");
        delay(750);
    }
}
