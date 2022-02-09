#include "main.h"
#include "definitions.hpp"
#include "debug.hpp"
#include "robot.hpp"
#include "screen.hpp"

#define FORCE_AUTONOMOUS false

void debug_input_task();

/**
 * Called when the robot is first initialized.
 */
void initialize() {
    Task::create(debug_input_task, "Debug Input Task");

    motor_rf.set_brake_mode(MOTOR_BRAKE_BRAKE);
    motor_rb.set_brake_mode(MOTOR_BRAKE_BRAKE);
    motor_lf.set_brake_mode(MOTOR_BRAKE_BRAKE);
    motor_lb.set_brake_mode(MOTOR_BRAKE_BRAKE);
    lift.set_brake_mode(MOTOR_BRAKE_HOLD);
    lift.set_zero_position(0);
    arm_1.set_brake_mode(MOTOR_BRAKE_HOLD);
    arm_2.set_brake_mode(MOTOR_BRAKE_HOLD);
    arm_1.set_zero_position(0);
    arm_2.set_zero_position(0);
}

void disabled() {
}

void competition_initialize() {
}

/**
 * Called when the robot is in it's autonomous state in a competition.
 * Or when the "Force Autonomous" button is presssed on the screen.
 */
void autonomous() {
    for (int i = 0; i < 3; i++) {
        lift_down();
        forwards(60, 100);
        lift_lift(100, true);
        backwards(60, 100);
        lift_up(100, true);
    }
}

/**
 * Called when the robot is under driver control.
 * Will delegate to autonomous control if the "Force Autonomous" button is pressed.
 */
[[noreturn]] void opcontrol() {
    if (FORCE_AUTONOMOUS) {
        autonomous();
    } else {
        while (true) {
            if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_R1))) {
                p_err(lift.move(127)); // UP
            } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_R2))) {
                p_err(lift.move(-127)); // DOWN
            } else {
                p_err(lift.move(0)); // STOP
            }
            if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_L1))) {
                move_arm(-127); // UP
            } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_L2))) {
                move_arm(127); // DOWN
            } else {
                move_arm(0); // STOP
            }
            if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_LEFT))) {
                p_err(arm_hook.move(-50)); // OPEN
            } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_RIGHT))) {
                p_err(arm_hook.move(50)); // SHUT
            } else {
                p_err(arm_hook.move(0)); // STOP
            }

            move_right_motors(p_err(controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y)));
            move_left_motors(p_err(controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)));

            delay(20);
        }
    }
}
