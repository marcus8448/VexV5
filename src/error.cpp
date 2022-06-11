#include "error.hpp"
#include <cerrno>
#include <cmath>
#include <cstring>

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \param return_code the return code of the pros function.
 * \return the same return_code parameter passed.
 */
int erri(int return_code) {
    if (return_code == INT32_MAX) {
        printf("Errno %i! '%s'", errno, std::strerror(errno));
    }
    return return_code;
}

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \param return_code the return code of the pros function.
 * \return the same return_code parameter passed.
 */
double errd(double return_code) {
    if (return_code == ((double) (INFINITY))) {
        printf("Errno %i! '%s'", errno, std::strerror(errno));
    }
    return return_code;
}
