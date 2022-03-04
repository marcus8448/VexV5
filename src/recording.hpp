#ifndef _RECORDING_H_
#define _RECORDING_H_

#include <fstream>
#include "devices.hpp"
#include "pros/misc.hpp"

void test() {
    std::basic_ofstream<unsigned char, std::char_traits<unsigned char>> outf("/usd/test.txt", std::ios::out | std::ios::binary | std::ios::app);
    outf << (signed char) -50;
    outf.flush();
}

void serialize_controller_state(std::basic_ofstream<unsigned char, std::char_traits<unsigned char>>& outf) {
    static bool a, b, x, y, up, down, left, right, l1, l2, r1, r2;
    static int lx, ly, rx, ry;

    if (a != (a = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))) {
        outf << (signed char)(a ? 1 : -1);
    } else if (b != (b = controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))) {
        outf << (signed char)(b ? 2 : -2);
    } else if (x != (x = controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))) {
        outf << (signed char)(x ? 3 : -3);
    } else if (y != (y = controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y))) {
        outf << (signed char)(y ? 4 : -4);
    } else if (up != (up = controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP))) {
        outf << (signed char)(up ? 5 : -5);
    } else if (down != (down = controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))) {
        outf << (signed char)(down ? 6 : -6);
    } else if (left != (left = controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))) {
        outf << (signed char)(left ? 7 : -7);
    } else if (right != (right = controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))) {
        outf << (signed char)(right ? 8 : -8);
    } else if (l1 != (l1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))) {
        outf << (signed char)(l1 ? 9 : -9);
    } else if (l2 != (l2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))) {
        outf << (signed char)(l2 ? 10 : -10);
    } else if (r1 != (r1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))) {
        outf << (signed char)(r1 ? 11 : -11);
    } else if (r2 != (r2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))) {
        outf << (signed char)(r2 ? 12 : -12);
    }
    outf << (signed char)13;
    outf << (signed char)lx;
    outf << (signed char)ly;
    outf << (signed char)rx;
    outf << (signed char)ry;
}

#endif //_RECORDING_H_