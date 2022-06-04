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

    bool a = false, b = false, x = false, y = false, up = false, down = false, left = false, right = false, l1 = false, l2 = false, r1 = false, r2 = false;
    double lx = 0.0, ly = 0.0, rx = 0.0, ry = 0.0;
    char c = '*';

    while (inf.peek() != EOF) {
        bool exit = false;
        while (!exit) {
            inf >> c;
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
                    inf.close();
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
    inf.close();
}
