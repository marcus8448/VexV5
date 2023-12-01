#include "error.hpp"
#include "debug/logger.hpp"
#include <cerrno>

namespace error {
void print(std::string_view name) {
  if (errno == ENODEV)
    return;
  logger::error("{}: Error {}", name, errno); // print the error
}

bool check(const float val) { return val == FLOATING; }

bool check(const double val) { return val == DOUBLE; }

bool check(const int32_t val) { return val == INTEGER; }

bool isDisconnected() { return errno == ENODEV; }
} // namespace error
