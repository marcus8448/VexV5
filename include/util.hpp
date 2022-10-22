#ifndef UTIL_HPP
#define UTIL_HPP

#include "pros/motors.hpp"

#define PI 3.14159265358979323846
#define WHEEL_SIZE 2.0625
#define WHEEL_CIRCUMFERENCE (WHEEL_SIZE * 2.0 * PI)

/**
 * Converts inches to encoder unit rotations.
 * @param inches the number of inches to convert.
 * @return the number of encoder units equivalent to the number of inches.
 */
double in_to_e_units(double inches);

/**
 * Converts degrees turning into encoder unit rotations.
 * @param degrees the number of degrees to convert.
 * @return the number of encoder units equivalent to the number of degrees.
 */
double turn_to_e_units(double degrees);

int32_t get_gearset_max_rpm(pros::motor_gearset_e_t gearset);

/**
 * Tests if a file exists on the microSD filesystem.
 * Only files under `/usd` are accessible.
 * @param name the name of the file
 * @return whether the file exists
 */
bool file_exists(const char *name);

#endif // UTIL_HPP
