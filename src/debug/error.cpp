#include "include/debug/error.hpp"
#include "include/debug/logger.hpp"
#include <cerrno>
#include <cstring>

bool check_error() {
  int error = get_error();
  if (error != 0) {
    if (error == ENODEV) { // skip printing 19 - no such device.
      return false;
    }
    error("Error %i: %s", error, strerror(error)); // print the error
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
