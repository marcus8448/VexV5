#ifndef VEXV5_UTIL_HPP
#define VEXV5_UTIL_HPP

#include "pros/motors.hpp"
#include <string>

#define PI 3.14159265358979323846
#define WHEEL_SIZE 2.0625
#define WHEEL_CIRCUMFERENCE (WHEEL_SIZE * 2.0 * PI)

double in_to_rot(double inches);

double turn_to_rot(double degrees);

std::string indent(const std::string& str);

std::string describe_motor(pros::Motor* motor);

void print(const char* string);
void print(std::string string);

void println(const char* string);
void println(std::string string);

void print_section(const char* string);
void print_section(std::string string);

#endif//VEXV5_UTIL_HPP
