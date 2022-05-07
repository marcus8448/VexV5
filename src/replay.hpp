#ifndef _REPLAY_HPP_
#define _REPLAY_HPP_

#include <fstream>
#include "pros/rtos.hpp"
#include "debug.hpp"
#include "opcontrol.hpp"
#include "robot.hpp"

void replay_match() {
    while (!usd::is_installed()) {
        controller.set_text(2, 0, "Missing microSD!");
        delay(250);
    }
    controller.clear_line(2);

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
        unsigned long long value;
        inf >> value;
        std::memcpy(&lx, &value, sizeof(lx));
        inf >> value;
        std::memcpy(&ly, &value, sizeof(ly));
        inf >> value;
        std::memcpy(&rx, &value, sizeof(rx));
        inf >> value;
        std::memcpy(&ry, &value, sizeof(ry));

        unsigned int digital_speed = 127;
        drive(a, b, x, y, up, down, left, right, l1, l2, r1, r2, lx, ly, rx, ry, &digital_speed);
        pros::delay(20);
    }
}

#endif //_REPLAY_HPP_
