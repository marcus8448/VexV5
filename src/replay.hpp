#ifndef _REPLAY_HPP_
#define _REPLAY_HPP_

#include <fstream>
#include "pros/rtos.hpp"
#include "debug.hpp"
#include "opcontrol.hpp"
#include "robot.hpp"

void replay_match() {
    std::basic_ifstream<signed int, std::char_traits<signed int>> inf("/usd/record.v5r", std::ios::in | std::ios::binary);

    bool a, b, x, y, up, down, left, right, l1, l2, r1, r2;
    int lx, ly, rx, ry;
    signed int i; 

    while (true) {
        bool exit = false;
        inf >> i;
        while (!exit) {
            switch (std::abs(i)) {
            case 1:
            a = i == 1;
            break;
            case 2:
            b = i == 2;
            break;
            case 3:
            x = i == 3;
            break;
            case 4:
            y = i == 4;
            break;
            case 5:
            up = i == 5;
            break;
            case 6:
            down = i == 6;
            break;
            case 7:
            left = i == 7;
            break;
            case 8:
            right = i == 8;
            break;
            case 9:
            l1 = i == 9;
            break;
            case 10:
            l2 = i == 10;
            break;
            case 11:
            r1 = i == 11;
            break;
            case 12:
            r2 = i == 12;
            break;
            case 13:
            exit = true;
            default:
                print_error("Invalid code!");
                print_error((int)i);
                return;
            }
        }
        inf >> i;
        lx = i;
        inf >> i;
        ly = i;
        inf >> i;
        rx = i;
        inf >> i;
        ry = i;

        unsigned int digital_speed = 127;
        unsigned int prev_digital_speed = 0;
        drive(a, b, x, y, up, down, left, right, l1, l2, r1, r2, lx, ly, rx, ry, &digital_speed, &prev_digital_speed);
        pros::delay(20);
    }
}

#endif //_REPLAY_HPP_