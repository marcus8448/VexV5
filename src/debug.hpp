#ifndef _DEBUG_TOOLS_HPP_
#define _DEBUG_TOOLS_HPP_

#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include "devices.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"

using namespace std;

bool driver_control = false;

void print(const char* line) {
    cout << line << endl;   
}

void print(const string& line) {
    print(line.c_str());
}

void print(int line) {
    print(to_string(line));
}

void print(unsigned int line) {
    print(to_string(line));
}

void print(double line) {
    print(to_string(line));
}

void print(long long line) {
    print(to_string(line));
}

void print_error(const char* line) {
    cerr << "\u001b[31m" << line << "\u001b[0m" << endl;   
}

void print_error(const string& line) {
    print_error(line.c_str());
}

void print_error(int line) {
    print_error(to_string(line));
}

void print_error(double line) {
    print_error(to_string(line));
}

void print_error(long long line) {
    print_error(to_string(line));
}

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \param return_code the return code of the pros function.
 * \return the same return_code parameter passed.
 */
int32_t p_err(int32_t return_code) {
    if (return_code == 2147483647) {
        print(std::strerror(errno));
    }
    return return_code;
}

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \param return_code the return code of the pros function.
 * \return the same return_code parameter passed.
 */
double p_err(double return_code) {
    if (return_code == ((float)(1e+300 * 1e+300))) {
        print_error(std::strerror(errno));
    }
    return return_code;
}

void print_motor_info(pros::Motor motor) {
    cout << motor.get_actual_velocity()
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
    cin.clear();
    while (true) {
        string command;
        getline(cin, command);
        if (command == "lift") {
            cout << "Lift: ";
            print_motor_info(lift);
        } else if (command == "arm") {
            cout << "Arm 1: ";
            print_motor_info(arm_1);
            cout << "\nArm 2: ";
            print_motor_info(arm_2);
            cout << "\nArm Hook: ";
            print_motor_info(arm_hook);
        } else if (command == "drivetrain") {
            cout << "RF Motor: ";
            print_motor_info(motor_rf);
            cout << "\nRB Motor: ";
            print_motor_info(motor_rb);
            cout << "\nLF Motor: ";
            print_motor_info(motor_lf);
            cout << "\nLB Motor: ";
            print_motor_info(motor_lb);
        }
        cout << endl;
    }
}

/**
 * 
 */
void controller_update_task() {
    while (true) {
        if (driver_control) {
            if (arm_hook.get_efficiency() < 15.0 && arm_hook.get_efficiency() > 2.0) {
                // p_err(controller.rumble("-"));
                controller.set_text(1, 0, "Resistance on hook!");
            } else {
                controller.clear_line(1);
            }
            delay(150);
        } else {
            p_err(controller.rumble(" "));
            delay(500);
        }
    }
    
}

#endif // _DEBUG_TOOLS_HPP_
