#ifndef DEBUG_ERROR_HPP
#define DEBUG_ERROR_HPP
#include "numeric"
#include <cstdint>

namespace error {
constexpr double INTEGER = std::numeric_limits<int32_t>::max();
constexpr double FLOATING = std::numeric_limits<double>::infinity();

/**
 * Tests for a PROS error and prints out errno if one is found.
 * @return whether an error was found.
 */
bool check_error(const char *name);
void print(const char *name);

bool check(double val);
bool check(int32_t val);

void clear();

int get_error();

/**
 * Tests for a PROS error and prints out errno if one is found.
 * @return the same value as the parameter passed.
 */
template <class T> inline T print(const char *name, T value) {
  check_error(name);
  return value;
}
}
#endif // DEBUG_ERROR_HPP
