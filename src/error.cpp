#include "error.hpp"
#include "logger.hpp"
#include <cerrno>
#include <cstring>

bool check_error() {
  if (errno != 0) {
    if (errno == ENODEV)
      return true; //ignore 19 - no such device.
    logger::error("Error %i: %s", errno, strerror(errno));
    errno = 0;
    return false;
  }
  return true;
}
