#include "debug/error.hpp"
#include "debug/logger.hpp"
#include <cerrno>

namespace error {
void print(const char *name) {
  if (errno == ENODEV) return;
  logger::error("%s: Error %i", name, errno); // print the error
}

bool check(double val) {
  return val == FLOATING;
}

bool check(int32_t val) {
  return val == INTEGER;
}

bool isDisconnected() { return errno == ENODEV; }
}
