#include "util.hpp"
#include <iostream>
#include <regex>

double in_to_rot(double inches) {
    return inches / WHEEL_CIRCUMFERENCE * 360.0;
}

double turn_to_rot(double degrees) {
    return degrees * 4.0;
}

void print(const char* string) {
    std::cout << string;
}

void print(std::string string) {
    std::cout << string;
}

void println(const char* string) {
    std::cout << string << std::endl;
}
void println(std::string string) {
    std::cout << string << std::endl;
}

void print_section(const char* string) {
    std::cout << "=== " << string << " ===" << std::endl;
}
void print_section(std::string string) {
    std::cout << "=== " << string << " ===" << std::endl;
}
