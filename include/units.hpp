#ifndef UNITS_HPP
#define UNITS_HPP

namespace units {
/**
 * Converts inches to encoder unit rotations.
 * @param inches the number of inches to convert.
 * @return the number of encoder units equivalent to the number of inches.
 */
double inchToEncoder(double inches);
double encoderToInch(double encoderUnits);

/**
 * Converts degrees turning into encoder unit rotations.
 * @param degrees the number of degrees to convert.
 * @return the number of encoder units equivalent to the number of degrees.
 */
double degreesToEncoder(double degrees);
double encoderToDegrees(double encoderUnits);
} // namespace units

#endif // UNITS_HPP
