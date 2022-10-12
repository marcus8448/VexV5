#ifndef UTIL_HPP
#define UTIL_HPP

#define PI 3.14159265358979323846
#define WHEEL_SIZE 2.0625
#define WHEEL_CIRCUMFERENCE (WHEEL_SIZE * 2.0 * PI)

double in_to_rot(double inches);

double turn_to_rot(double degrees);

/**
 * Tests if a file exists on the microSD filesystem.
 * All files must exist under "/usd/", otherwise VexOS will block the call.
 * \param name the name of the file
 * \return whether the file exists
 */
bool file_exists(const char *name);

#endif // UTIL_HPP
