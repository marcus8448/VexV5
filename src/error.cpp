#include "error.hpp"
#include "logger.hpp"
#include <cerrno>
#include <cstring>

bool check_error() {
  if (errno != 0) {
    if (errno == ENODEV) {
      errno = 0;    // reset errno so that we don't keep on detecting the same error.
      return false; // skip printing 19 - no such device.
    }
    logger::error("Error %i: %s", errno, strerror(errno)); // print the error
    errno = 0; // reset errno so that we don't keep on detecting the same error.
    return false;
  }
  return true;
}
