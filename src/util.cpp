#include "util.hpp"
#include <cstdio>

double in_to_rot(double inches) {
  return inches / WHEEL_CIRCUMFERENCE * 360.0;
}

double turn_to_rot(double degrees) {
  return degrees * 4.0;
}

// https://stackoverflow.com/a/12774387
bool file_exists(const char *name) {
  if (FILE *file = std::fopen(name, "r")) {
    std::fclose(file);
    return true;
  } else {
    return false;
  }
}
