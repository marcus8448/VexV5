#ifndef DEBUG_ERROR_HPP
#define DEBUG_ERROR_HPP

/**
 * Tests for a PROS error and prints out errno if one is found.
 * @return whether an error was found.
 */
bool check_error(const char *name);

void clear_error();

int get_error();

/**
 * Tests for a PROS error and prints out errno if one is found.
 * @return the same value as the parameter passed.
 */
template <class T> inline T print_error(const char *name, T value) {
  check_error(name);
  return value;
}

#endif // DEBUG_ERROR_HPP
