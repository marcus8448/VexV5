#include "util.hpp"
#include <cstdio>

#define PI 3.14159265358979323846
#define WHEEL_SIZE 2.0625
#define WHEEL_CIRCUMFERENCE (WHEEL_SIZE * 2.0 * PI)

double in_to_e_units(double inches) { return inches / WHEEL_CIRCUMFERENCE * 360.0; }

double turn_to_e_units(double degrees) { return degrees * 3.22; }

// https://stackoverflow.com/a/12774387
bool file_exists(const char *name) {
  if (std::FILE *file = std::fopen(name, "r")) {
    std::fclose(file);
    return true;
  } else {
    return false;
  }
}
