#ifndef _REPLAY_H_
#define _REPLAY_H_

#include <fstream>
#include "pros/rtos.hpp"
#include "robot.hpp"
#include "screen.hpp"

void replay_match() {
    static std::ifstream inf("/usd/record.v5r", std::ios::in | std::ios::binary);

    static bool a, b, x, y, up, down, left, right, l1, l2, r1, r2;
    static int lx, ly, rx, ry;
    char c; 

    if (inf.get(c)) {
        if (c != 127) {
            print_error("Invalid starting character!");
            return;
        }
    }

    while (true) {
        while (inf.get(c)) {
            switch (c)
            {
            case 127:
                continue;
            case 
            default:
                print_error("Invalid code!");
                break;
            }
            if (inf.peek() == 127) {
                break;
            }
        }
        pros::delay(20);
    }
}

#endif //_REPLAY_H_