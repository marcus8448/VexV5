#include "util.hpp"
#include <iostream>
#include <regex>

double in_to_rot(double inches) {
    return inches / WHEEL_CIRCUMFERENCE * 360.0;
}

double turn_to_rot(double degrees) {
    return degrees * 4.0;
}
