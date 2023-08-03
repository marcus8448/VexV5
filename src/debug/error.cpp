#include "debug/error.hpp"
#include "debug/logger.hpp"
#include <cerrno>

bool check_error(const char *name) {
  int error = get_error();
  if (error != 0) {
    if (error == ENODEV) { // skip printing 19 - no such device.
      return false;
    }
    error("%s: Error %i", name, error); // print the error
    return false;
  }
  return true;
}

void clear_error() { errno = 0; }

int get_error() {
  int error = errno;
  errno = 0;
  return error;
}
