#ifndef VEXV5_DEBUG_TOOLS_HPP
#define VEXV5_DEBUG_TOOLS_HPP

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \return the same return_code parameter passed.   
 */
bool check_error();

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \return the same return_code parameter passed.
 */
template<class T> T check_error(T value) {
    check_error();
    return value;
}

#endif//VEXV5_DEBUG_TOOLS_HPP
