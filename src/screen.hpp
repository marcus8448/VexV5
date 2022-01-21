#ifndef _SCREEN_MANAGEMENT_H_
#define _SCREEN_MANAGEMENT_H_

#include "definitions.hpp"
#include <string>

static lv_obj_t* static_information = nullptr;
static lv_obj_t* printed_information = nullptr;

static int lineNum = 0;

void init_screen() {
}

void init_btn(lv_obj_t* btn) {
    lv_obj_set_height(btn, lv_obj_get_height(btn) - lv_obj_get_height(btn) / 4);
    lv_obj_set_click(btn, false);
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
    std::cerr << line << std::endl;   
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
