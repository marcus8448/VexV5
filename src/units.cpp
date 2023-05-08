#include "units.hpp"
#include <cmath>

#define WHEEL_SIZE 2.0625
#define WHEEL_CIRCUMFERENCE (WHEEL_SIZE * 2.0 * M_PI)
#define TURN_E_UNITS_RATIO 2.988

namespace units {
double inchToEncoder(double inches) { return inches / WHEEL_CIRCUMFERENCE * 360.0; }
double encoderToInch(double encoderUnits) { return (encoderUnits * WHEEL_CIRCUMFERENCE) / 360.0; }

double degreesToEncoder(double degrees) { return degrees * TURN_E_UNITS_RATIO; }
double encoderToDegrees(double encoderUnits) { return encoderUnits / TURN_E_UNITS_RATIO; }

} // namespace util
