#include "util.hpp"
#include <regex>

double in_to_rot(double inches) {
    return inches / WHEEL_CIRCUMFERENCE * 360.0;
}

double turn_to_rot(double degrees) {
    return degrees * 4.0;
}

std::string indent(const std::string& str){
    static auto regex = std::basic_regex("\n");
    return std::regex_replace(str, regex, "\n  ");
}

std::string describe_motor(pros::Motor* motor) {
    return std::to_string(motor->get_actual_velocity()).append("RPM / ").append(std::to_string(motor->get_target_velocity())).append("RPM | ")
            .append(std::to_string(motor->get_position())).append("° / ").append(std::to_string(motor->get_target_position())).append("° | ")
            .append(std::to_string(motor->get_voltage())).append("V | ").append(std::to_string(motor->get_efficiency())).append("%");
}
