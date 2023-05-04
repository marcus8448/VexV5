#ifndef DEBUG_ERROR_HPP
#define DEBUG_ERROR_HPP

/**
 * Tests for a PROS error and prints out errno if one is found.
 * @return whether an error was found.
 */
bool check_error();

void clear_error();

int get_error();

/**
 * Tests for a PROS error and prints out errno if one is found.
 * @return the same value as the parameter passed.
 */
template <class T> T print_error(T value) {
  check_error();
  return value;
}

#endif // DEBUG_ERROR_HPP
