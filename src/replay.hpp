#ifndef _REPLAY_H_
#define _REPLAY_H_

#include <fstream>
#include "pros/rtos.hpp"
#include "robot.hpp"
#include "debug.hpp"

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

        int digital_speed = 127;
        int prev_digital_speed = 0;
        int cooldown = 0;
        bool lift_lock = false;
        bool arm_lock = false;

        if (cooldown > 0) {
            cooldown--;
        } else if (cooldown < 0) {
            cooldown = 0;
        }
        if (r1) {
            p_err(lift.move(digital_speed)); // UP
            lift_lock = false;
        } else if (r2) {
            p_err(lift.move(-digital_speed)); // DOWN
            lift_lock = false;
        } else {
            if (!lift_lock) {
                p_err(lift.move(0)); // STOP
            }
        }
        if (l1) {
            move_arm(-digital_speed); // UP
            arm_lock = false;
        } else if (l2) {
            move_arm(digital_speed); // DOWN
            arm_lock = false;
        } else {
            if (!arm_lock) {
                move_arm(0); // STOP
            }
        }

        if (left) {
            p_err(arm_hook.move(-100)); // OPEN
        } else if (right) {
            p_err(arm_hook.move(100)); // SHUT
        } else {
            p_err(arm_hook.move(0)); // STOP
        }

        if (up) {
            if (digital_speed + 1 <= 127) {
                digital_speed += 1;
                print(digital_speed);
            }
        } else if (down) {
            if (digital_speed - 1 > 20) {
                digital_speed -= 1;
                print(digital_speed);
            }
        }

        if (a && y) {
            arm_down(200, false);
            arm_lock = true;
            cooldown = 20;
        } else if (cooldown == 0) {
            if (a) {
                arm_up(200, false);
                arm_lock = true;
            } else if (y) {
                arm_prime(200, false);
                arm_lock = true;
            }
        }

        if (x && b) {
            lift_lift(200, false);
            lift_lock = true;
            cooldown = 20;
        } else if (cooldown == 0) {
            if (x) {
                lift_up(200, false);
                lift_lock = true;
            } else if (b) {
                lift_down(200, false);
                lift_lock = true;
            }
        }

        move_right_motors(ry);
        move_left_motors(ly);
        pros::delay(20);
    }
}

#endif //_REPLAY_H_