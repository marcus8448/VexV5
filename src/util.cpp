#include "util.hpp"
#include <cstdio>

#define PI 3.14159265358979323846
#define WHEEL_SIZE 2.0625
#define WHEEL_CIRCUMFERENCE (WHEEL_SIZE * 2.0 * PI)

namespace util {
double in_to_e_units(double inches) { return inches / WHEEL_CIRCUMFERENCE * 360.0; }
double e_units_to_in(double e_units) { return (e_units * WHEEL_CIRCUMFERENCE) / 360.0; }

double turn_to_e_units(double degrees) { return degrees * 3.25; }
double e_units_to_turn(double e_units) { return e_units / 3.25; }

// https://stackoverflow.com/a/12774387
bool file_exists(const char *name) {
  if (FILE *file = fopen(name, "r")) {
    fclose(file);
    return true;
  } else {
    return false;
  }
}
}

