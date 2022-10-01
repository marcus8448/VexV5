#ifndef DEBUG_TOOLS_HPP
#define DEBUG_TOOLS_HPP

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \return the same return_code parameter passed.   
 */
bool check_error();

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \return the same value as the parameter passed.
 */
template<class T> T check_error(T value) {
  check_error();
  return value;
}

#endif//DEBUG_TOOLS_HPP
