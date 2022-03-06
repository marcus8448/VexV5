#ifndef _RECORDING_H_
#define _RECORDING_H_

#include <fstream>
#include "devices.hpp"
#include "pros/misc.hpp"

void test() {
    std::basic_ofstream<signed int, std::char_traits<signed int>> outf("/usd/test.txt", std::ios::out | std::ios::binary | std::ios::app);
    outf << -50;
    outf.flush();
}

void serialize_controller_state(std::basic_ofstream<signed int, std::char_traits<signed int>>& outf) {
    static bool a, b, x, y, up, down, left, right, l1, l2, r1, r2;

    if (a != (a = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))) {
        outf << (signed int)(a ? 1 : -1);
    } else if (b != (b = controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))) {
        outf << (signed int)(b ? 2 : -2);
    } else if (x != (x = controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))) {
        outf << (signed int)(x ? 3 : -3);
    } else if (y != (y = controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y))) {
        outf << (signed int)(y ? 4 : -4);
    } else if (up != (up = controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP))) {
        outf << (signed int)(up ? 5 : -5);
    } else if (down != (down = controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))) {
        outf << (signed int)(down ? 6 : -6);
    } else if (left != (left = controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))) {
        outf << (signed int)(left ? 7 : -7);
    } else if (right != (right = controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))) {
        outf << (signed int)(right ? 8 : -8);
    } else if (l1 != (l1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))) {
        outf << (signed int)(l1 ? 9 : -9);
    } else if (l2 != (l2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))) {
        outf << (signed int)(l2 ? 10 : -10);
    } else if (r1 != (r1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))) {
        outf << (signed int)(r1 ? 11 : -11);
    } else if (r2 != (r2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))) {
        outf << (signed int)(r2 ? 12 : -12);
    }
    outf << (signed int)13;
    outf << (signed int)controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
    outf << (signed int)controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    outf << (signed int)controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    outf << (signed int)controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
}

#endif //_RECORDING_H_