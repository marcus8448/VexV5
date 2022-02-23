#ifndef _RECORDING_H_
#define _RECORDING_H_

#include <fstream>
#include "robot.hpp"
#include "pros/misc.hpp"

void write_out(char c) {
    static std::ofstream outf("/usd/record.v5r", std::ios::out | std::ios::binary || std::ios::trunc);
    outf << c;
}

void serialize_controller_state() {
    static bool a, b, x, y, up, down, left, right, l1, l2, r1, r2;
    static int lx, ly, rx, ry;

    write_out(127);
    if (a != (a = controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))) {
        write_out(a ? 1 : -1);
    } else if (b != (b = controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))) {
        write_out(b ? 2 : -2);
    } else if (x != (x = controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))) {
        write_out(x ? 3 : -3);
    } else if (y != (y = controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y))) {
        write_out(y ? 4 : -4);
    } else if (up != (up = controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP))) {
        write_out(up ? 5 : -5);
    } else if (down != (down = controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))) {
        write_out(down ? 6 : -6);
    } else if (left != (left = controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))) {
        write_out(left ? 7 : -7);
    } else if (right != (right = controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))) {
        write_out(right ? 8 : -8);
    } else if (l1 != (l1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))) {
        write_out(l1 ? 9 : -9);
    } else if (l2 != (l2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))) {
        write_out(l2 ? 10 : -10);
    } else if (r1 != (r1 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))) {
        write_out(r1 ? 11 : -11);
    } else if (r2 != (r2 = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))) {
        write_out(r2 ? 12 : -12);
    }
    write_out(13);
    if (lx != (lx = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X))) {
        write_out(lx);
    } else if (ly != (ly = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y))) {
        write_out(ly);
    } else if (rx != (rx = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X))) {
        write_out(rx);
    } else if (ry != (ry = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y ))) {
        write_out(ry);
    }
}

#endif //_RECORDING_H_