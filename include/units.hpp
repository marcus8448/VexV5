#ifndef UNITS_HPP
#define UNITS_HPP

#include <cmath>

#define WHEEL_RADIUS 2.0625
#define WHEEL_CIRCUMFERENCE (WHEEL_RADIUS * 2.0 * M_PI)
#define TURN_E_UNITS_RATIO 2.988
#define DRIVETRAIN_GEARING (36.0 / 84.0)
#define DRIVETRAIN_WHEEL_DIFF units::inchToEncoder(11.5)
#define DRIVETRAIN_CORE_DIFF units::inchToEncoder(11.5 / 2.0)

namespace units {
/**
 * Converts inches to encoder units (degrees).
 * @param inches the number of inches to convert.
 * @return the number of encoder units equivalent to the number of inches.
 */
inline double inchToEncoder(double inches) { return inches / (WHEEL_CIRCUMFERENCE * DRIVETRAIN_GEARING) * 360.0; }
inline double encoderToInch(double encoderUnits) {
  return (encoderUnits * (WHEEL_CIRCUMFERENCE * DRIVETRAIN_GEARING)) / 360.0;
}

/**
 * Converts degrees turning into encoder unit rotations.
 * @param degrees the number of degrees to convert.
 * @return the number of encoder units equivalent to the number of degrees.
 */
inline double degreesToEncoder(double degrees) { return degrees * TURN_E_UNITS_RATIO; }
inline double encoderToDegrees(double encoderUnits) { return encoderUnits / TURN_E_UNITS_RATIO; }
} // namespace units

#endif // UNITS_HPP
