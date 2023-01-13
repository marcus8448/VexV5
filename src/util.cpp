#include "util.hpp"

#define PI 3.14159265358979323846
#define WHEEL_SIZE 2.0625
#define WHEEL_CIRCUMFERENCE (WHEEL_SIZE * 2.0 * PI)
#define TURN_E_UNITS_RATIO 2.988

namespace util {
double in_to_e_units(double inches) { return inches / WHEEL_CIRCUMFERENCE * 360.0; }
double e_units_to_in(double e_units) { return (e_units * WHEEL_CIRCUMFERENCE) / 360.0; }

double turn_to_e_units(double degrees) { return degrees * TURN_E_UNITS_RATIO; }
double e_units_to_turn(double e_units) { return e_units / TURN_E_UNITS_RATIO; }

} // namespace util
