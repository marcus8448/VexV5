#include "debug/error.hpp"
#include "debug/logger.hpp"
#include <cerrno>

namespace error {
bool check_error(const char *name) {
  int error = get_error();
  if (error != 0) {
    if (error == ENODEV) { // skip printing 19 - no such device.
      return false;
    }
    logger::error("%s: Error %i", name, error); // print the error
    return false;
  }
  return true;
}

void print(const char *name) {
  int error = errno;
  if (error == 0) return;
  logger::error("%s: Error %i", name, error); // print the error
}

bool check(double val) {
  return val == FLOATING;
}

bool check(int32_t val) {
  return val == INTEGER;
}

void clear() { errno = 0; }

int get_error() {
  int error = errno;
  errno = 0;
  return error;
}
}
