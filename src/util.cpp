#include "util.hpp"
#include "pros/motors.h"
#include <cstdio>

double in_to_e_units(double inches) { return inches / WHEEL_CIRCUMFERENCE * 360.0; }

double turn_to_e_units(double degrees) { return degrees * 3.37; }

// https://stackoverflow.com/a/12774387
bool file_exists(const char *name) {
  if (std::FILE *file = std::fopen(name, "r")) {
    std::fclose(file);
    return true;
  } else {
    return false;
  }
}
