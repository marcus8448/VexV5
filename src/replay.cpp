#include <cstring>
#include <fstream>
#include "pros/rtos.hpp"
#include "vexv5/debug.hpp"
#include "vexv5/replay.hpp"
#include "vexv5/robot.hpp"

ReplayController::ReplayController() {

}

bool ReplayController::a_pressed() {

}
bool ReplayController::b_pressed() {

}
bool ReplayController::x_pressed() {

}
bool ReplayController::y_pressed() {

}

bool ReplayController::up_pressed() {

}
bool ReplayController::down_pressed() {

}
bool ReplayController::left_pressed() {

}
bool ReplayController::right_pressed() {

}

bool ReplayController::l1_pressed() {

}
bool ReplayController::l2_pressed() {

}
bool ReplayController::r1_pressed() {

}
bool ReplayController::r2_pressed() {

}

unsigned short int ReplayController::a_press_length() {

}
unsigned short int ReplayController::b_press_length() {

}
unsigned short int ReplayController::x_press_length() {

}
unsigned short int ReplayController::y_press_length() {

}

unsigned short int ReplayController::up_press_length() {

}
unsigned short int ReplayController::down_press_length() {

}
unsigned short int ReplayController::left_press_length() {

}
unsigned short int ReplayController::right_press_length() {

}

unsigned short int ReplayController::l1_press_length() {

}
unsigned short int ReplayController::l2_press_length() {

}
unsigned short int ReplayController::r1_press_length() {

}
unsigned short int ReplayController::r2_press_length() {

}

unsigned char ReplayController::get_digital_speed() {

}
void ReplayController::set_digital_speed(unsigned char digitalSpeed) {

}

void ReplayController::set_line(unsigned char line, unsigned char col, const char* str) {

}
void ReplayController::clear_line(unsigned char line) {

}

void ReplayController::rumble(const char* str) {

}

double ReplayController::left_stick_x() {

}
double ReplayController::left_stick_y() {

}
double ReplayController::right_stick_x() {

}
double ReplayController::right_stick_y() {

}

double ReplayController::prev_left_stick_x() {

}
double ReplayController::prev_left_stick_y() {

}
double ReplayController::prev_right_stick_x() {

}
double ReplayController::prev_right_stick_y() {

}

void ReplayController::update() {
    if (this->inf.peek() == EOF) {
        reset();
    }

    char c = '*';

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
}

void reset() {

}
void stop() {

}

void replay_match(Robot robot) {
    while (!pros::usd::is_installed()) {
        robot.controller.set_line(2, 0, "Missing microSD!");
        pros::delay(250);
    }
    robot.controller.clear_line(2);

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
        robot
        drive(a, b, x, y, up, down, left, right, l1, l2, r1, r2, lx, ly, rx, ry, &digital_speed);
        pros::delay(20);
    }
    inf.close();
}
