#include <cstring>
#include <fstream>
#include "pros/rtos.hpp"
#include "vexv5/debug.hpp"
#include "vexv5/globals.hpp"
#include "vexv5/opcontrol.hpp"
#include "vexv5/robot.hpp"

void replay_match() {
    while (!pros::usd::is_installed()) {
        controller.set_text(2, 0, "Missing microSD!");
        delay(250);
    }
    controller.clear_line(2);

    std::ifstream inf("/usd/record.v5r", std::ios::in | std::ios::binary);

    bool a, b, x, y, up, down, left, right, l1, l2, r1, r2;
    int lx, ly, rx, ry;
    char c; 

    while (true) {
        bool exit = false;
        inf >> c;
        while (!exit) {
            switch (c) {
                case 'a':
                    a = !a;
                    break;
                case 'b':
                    b = !b;
                    break;
                case 'x':
                    x = !x;
                    break;
                case 'y':
                    y = !y;
                    break;
                case 'u':
                    up = !up;
                    break;
                case 'd':
                    down = !down;
                    break;
                case 'l':
                    left = !left;
                    break;
                case 'r':
                    right = !right;
                    break;
                case '!':
                    l1 = !l1;
                    break;
                case '@':
                    l2 = !l2;
                    break;
                case '#':
                    r1 = !r1;
                    break;
                case '$':
                    r2 = !r2;
                    break;
                case '*':
                    exit = true;
                    break;
                default:
                    print_error("Invalid code!");
                    print_error(c);
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
