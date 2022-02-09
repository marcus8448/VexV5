#ifndef _DEBUG_TOOLS_H_
#define _DEBUG_TOOLS_H_

#include <iostream>
#include "robot.hpp"
#include "main.h"

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
        if (command == "lift") {
            std::cout << "Lift:\n";
            print_motor_info(lift);
        } else if (command == "arm") {
            std::cout << "Arm 1:\n";
            print_motor_info(arm_1);
            std::cout << "Arm 2:\n";
            print_motor_info(arm_2);
            std::cout << "Arm Hook:\n";
            print_motor_info(arm_hook);
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

#endif // _DEBUG_TOOLS_H_
