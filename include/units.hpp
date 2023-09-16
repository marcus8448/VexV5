#ifndef UNITS_HPP
#define UNITS_HPP
#include <numbers>

namespace units {
constexpr double WHEEL_DIAMETER = 3.25;
constexpr double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * std::numbers::pi;
constexpr double DRIVETRAIN_GEARING = 36.0 / 60.0;

/**
 * Converts inches to encoder units (degrees).
 * @param inches the number of inches to convert.
 * @return the number of encoder units equivalent to the number of inches.
 */
inline double inchToEncoder(double inches) { return inches / (WHEEL_CIRCUMFERENCE * DRIVETRAIN_GEARING) * 360.0; }
inline double encoderToInch(double encoderUnits) {
  return (encoderUnits / 360.0) * (WHEEL_CIRCUMFERENCE * DRIVETRAIN_GEARING);
}
} // namespace units

#endif // UNITS_HPP
