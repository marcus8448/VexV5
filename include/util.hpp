#ifndef UTIL_HPP
#define UTIL_HPP

#include "pros/motors.hpp"

namespace util {
/**
 * Converts inches to encoder unit rotations.
 * @param inches the number of inches to convert.
 * @return the number of encoder units equivalent to the number of inches.
 */
double in_to_e_units(double inches);
double e_units_to_in(double e_units);

/**
 * Converts degrees turning into encoder unit rotations.
 * @param degrees the number of degrees to convert.
 * @return the number of encoder units equivalent to the number of degrees.
 */
double turn_to_e_units(double degrees);
double e_units_to_turn(double e_units);
int32_t get_gearset_max_rpm(pros::motor_gearset_e_t gearset);
/**
 * Tests if a file exists on the microSD filesystem.
 * Only files under `/usd` are accessible.
 * @param name the name of the file
 * @return whether the file exists
 */
bool file_exists(const char *name);
} // namespace util

#endif // UTIL_HPP
