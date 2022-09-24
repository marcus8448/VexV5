#ifndef UTIL_HPP
#define UTIL_HPP

#include "pros/motors.hpp"
#include <string>

#define PI 3.14159265358979323846
#define WHEEL_SIZE 2.0625
#define WHEEL_CIRCUMFERENCE (WHEEL_SIZE * 2.0 * PI)

double in_to_rot(double inches);

double turn_to_rot(double degrees);

#endif//UTIL_HPP
