#ifndef _DEBUG_TOOLS_H_
#define _DEBUG_TOOLS_H_

#include <iostream>
#include <string>
#include "devices.hpp"
#include "pros/motors.hpp"

void print_motor_info(Motor motor) {
    std::cout << motor.get_actual_velocity()
    << " / " << motor.get_target_velocity()
    << " | " << motor.get_position()
    << "° / " << motor.get_target_position()
    << "° | " << motor.get_voltage()
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
            std::cout << "Lift: ";
            print_motor_info(lift);
        } else if (command == "arm") {
            std::cout << "Arm 1: ";
            print_motor_info(arm_1);
            std::cout << "\nArm 2: ";
            print_motor_info(arm_2);
            std::cout << "\nArm Hook: ";
            print_motor_info(arm_hook);
        } else if (command == "drivetrain") {
            std::cout << "RF Motor: ";
            print_motor_info(motor_rf);
            std::cout << "\nRB Motor: ";
            print_motor_info(motor_rb);
            std::cout << "\nLF Motor: ";
            print_motor_info(motor_lf);
            std::cout << "\nLB Motor: ";
            print_motor_info(motor_lb);
        }
        std::cout << std::endl;
    }
}

void print(const char* line) {
    std::cout << line << std::endl;   
}

void print(const std::string& line) {
    print(line.c_str());
}

void print(int line) {
    print(std::to_string(line));
}

void print(unsigned int line) {
    print(std::to_string(line));
}

void print(double line) {
    print(std::to_string(line));
}

void print(long long line) {
    print(std::to_string(line));
}

void print_error(const char* line) {
    std::cerr << "\u001b[31m" << line << "\u001b[0m" << std::endl;   
}

void print_error(const std::string& line) {
    print_error(line.c_str());
}

void print_error(int line) {
    print_error(std::to_string(line));
}

void print_error(double line) {
    print_error(std::to_string(line));
}

void print_error(long long line) {
    print_error(std::to_string(line));
}

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \param return_code the return code of the pros function.
 * \return the same return_code parameter passed.
 */
int32_t p_err(int32_t return_code) {
    if (return_code == PROS_ERR) {
        print(strerror(errno));
    }
    return return_code;
}

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \param return_code the return code of the pros function.
 * \return the same return_code parameter passed.
 */
double p_err(double return_code) {
    if (return_code == PROS_ERR_F) {
        print_error(strerror(errno));
    }
    return return_code;
}

#endif // _DEBUG_TOOLS_H_
