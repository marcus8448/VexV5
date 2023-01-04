#include "error.hpp"
#include "logger.hpp"
#include <cerrno>
#include <cstring>

bool check_error() {
  int32_t error = get_error();
  if (error != 0) {
    if (error == ENODEV) {
      return false;
    }                                              // skip printing 19 - no such device.
    error("Error %i: %s", error, strerror(error)); // print the error
    return false;
  }
  return true;
}

void clear_error() { errno = 0; }

int32_t get_error() {
  int32_t error = errno;
  errno = 0;
  return error;
}
