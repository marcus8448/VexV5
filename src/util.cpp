#include "util.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include <cstdio>

double in_to_e_units(double inches) { return inches / WHEEL_CIRCUMFERENCE * 360.0; }

double turn_to_e_units(double degrees) { return degrees * 3.37; }

double get_gearset_max_rpm(const pros::motor_gearset_e_t gearset) {
  switch (gearset) {
  case pros::E_MOTOR_GEARSET_36:
    return 100.0;
  case pros::E_MOTOR_GEARSET_18:
    return 200.0;
  case pros::E_MOTOR_GEARSET_06:
    return 600.0;
  case pros::E_MOTOR_GEARSET_INVALID:
    return -0.0;
  }
  return 0.0;
}

// https://stackoverflow.com/a/12774387
bool file_exists(const char *name) {
  if (std::FILE *file = std::fopen(name, "r")) {
    std::fclose(file);
    return true;
  } else {
    return false;
  }
}
