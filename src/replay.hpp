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

    while (true) {
        bool exit = false;
        while (inf.get(c) && !exit) {
            switch (std::abs(c)) {
            case 1:
            a = c == 1;
            break;
            case 2:
            b = c == 2;
            break;
            case 3:
            x = c == 3;
            break;
            case 4:
            y = c == 4;
            break;
            case 5:
            up = c == 5;
            break;
            case 6:
            down = c == 6;
            break;
            case 7:
            left = c == 7;
            break;
            case 8:
            right = c == 8;
            break;
            case 9:
            l1 = c == 9;
            break;
            case 10:
            l2 = c == 10;
            break;
            case 11:
            r1 = c == 11;
            break;
            case 12:
            r2 = c == 12;
            break;
            case 13:
            exit = true;
            default:
                print_error("Invalid code!");
                print_error((int)c);
                return;
            }
        }
        inf.get(c);
        lx = c;
        inf.get(c);
        ly = c;
        inf.get(c);
        rx = c;
        inf.get(c);
        ry = c;

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