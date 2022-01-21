#include "main.h"
#include "definitions.hpp"
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
    arm.set_brake_mode(MOTOR_BRAKE_BRAKE);
    arm.set_zero_position(0);
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
        arm_down();
        forwards(60, 100);
        arm_lift(100, true);
        backwards(60, 100);
        arm_up(100, true);
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
            int joystickL = p_err(controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
            int joystickR = p_err(controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y));
            if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_R1))/*  && p_err(arm_position()) < 300.0 */) {
                p_err(arm.move(127)); // UP
            } else if (p_err(controller.get_digital(E_CONTROLLER_DIGITAL_R2))/*  && p_err(arm_position()) > 0.0 */) {
                p_err(arm.move(-127)); // DOWN
            } else {
                p_err(arm.move(0)); // STOP
            }

            p_err(motor_rf.move(joystickR));
            p_err(motor_rb.move(joystickR));
            p_err(motor_lf.move(joystickL));
            p_err(motor_lb.move(joystickL));

            delay(20);
        }
    }
}

void print_motor_info(Motor motor) {
    std::cout << motor.get_actual_velocity()
    << " / " << motor.get_target_velocity()
    << " " << motor.get_position()
    << "° / " << motor.get_target_position()
    << "° " << motor.get_voltage()
    << "V";
}

/**
 * Handles debug commands.
 */
void debug_input_task() {
    std::cin.clear();
    while (true) {
        std::string command;
        std::getline(std::cin, command);
        if (command == "arm") {
            std::cout << "Arm:\n";
            print_motor_info(arm);
        } else if (command == "drivetrain") {
            std::cout << "RF Motor:\n";
            print_motor_info(motor_rf);
            std::cout << "RB Motor:\n";
            print_motor_info(motor_rb);
            std::cout << "LF Motor:\n";
            print_motor_info(motor_lf);
            std::cout << "LB Motor:\n";
            print_motor_info(motor_lb);
        }
        std::cout << std::endl;
    }
}
