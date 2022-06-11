#ifndef VEXV5_DEBUG_TOOLS_HPP
#define VEXV5_DEBUG_TOOLS_HPP

#include <string>

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \param return_code the return code of the pros function.
 * \return the same return_code parameter passed.
 */
int erri(int return_code);

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \param return_code the return code of the pros function.
 * \return the same return_code parameter passed.
 */
double errd(double return_code);

#endif//VEXV5_DEBUG_TOOLS_HPP
