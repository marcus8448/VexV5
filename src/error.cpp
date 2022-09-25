#include "error.hpp"
#include "logger.hpp"
#include <cerrno>
#include <cstring>

bool check_error() {
  if (errno != 0) {
    logger::error("Error: ", strerror(errno));
    errno = 0;
    return false;
  }
  return true;
}
