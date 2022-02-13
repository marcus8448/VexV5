#ifndef _SCREEN_MANAGEMENT_H_
#define _SCREEN_MANAGEMENT_H_

#include "definitions.hpp"
#include <string>

void init_screen() {
}

void print(const char* line) {
    std::cout << line << std::endl;   
}

void print(const std::string& line) {
    print(line.c_str());
}

void print(int line) {
    print(std::to_string(line));
}

void print(double line) {
    print(std::to_string(line));
}

void print(long long line) {
    print(std::to_string(line));
}

void print_error(const char* line) {
    std::cerr << "\u001b[31m" << line << "\u001b[0m" << std::endl;   
}

void print_error(const std::string& line) {
    print_error(line.c_str());
}

void print_error(int line) {
    print_error(std::to_string(line));
}

void print_error(double line) {
    print_error(std::to_string(line));
}

void print_error(long long line) {
    print_error(std::to_string(line));
}

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \param return_code the return code of the pros function.
 * \return the same return_code parameter passed.
 */
int32_t p_err(int32_t return_code) {
    if (return_code == PROS_ERR) {
        print(strerror(errno));
    }
    return return_code;
}

/**
 * Tests for a PROS error and prints out errno if one is found.
 * \param return_code the return code of the pros function.
 * \return the same return_code parameter passed.
 */
double p_err(double return_code) {
    if (return_code == PROS_ERR_F) {
        print_error(strerror(errno));
    }
    return return_code;
}

#endif // _SCREEN_MANAGEMENT_H_
