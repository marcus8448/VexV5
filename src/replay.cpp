#include "replay.hpp"
#include "error.hpp"
#include "pros/rtos.hpp"
#include "robot.hpp"
#include <cstring>
#include <fstream>

ReplayController::ReplayController(const char* filename) {
    auto controller = pros::Controller(pros::E_CONTROLLER_MASTER);
    while (!pros::usd::is_installed()) {
        controller.set_text(2, 0, "Missing microSD!");
        pros::delay(250);
    }
    controller.clear_line(2);

    this->inf = std::ifstream(filename, std::ios::in | std::ios::binary);
    this->inf >> this->type;
}

ReplayController::~ReplayController() {
    if (this->inf.is_open()) {
        this->inf.close();
    }
}

unsigned short int ReplayController::a_pressed() {
    return this->a;
}

unsigned short int ReplayController::b_pressed() {
    return this->b;
}

unsigned short int ReplayController::x_pressed() {
    return this->x;
}

unsigned short int ReplayController::y_pressed() {
    return this->y;
}

unsigned short int ReplayController::up_pressed() {
    return this->up;
}

unsigned short int ReplayController::down_pressed() {
    return this->down;
}
unsigned short int ReplayController::left_pressed() {
    return this->left;
}

unsigned short int ReplayController::right_pressed() {
    return this->right;
}

unsigned short int ReplayController::l1_pressed() {
    return this->l1;
}

unsigned short int ReplayController::l2_pressed() {
    return this->l2;
}

unsigned short int ReplayController::r1_pressed() {
    return this->r1;
}

unsigned short int ReplayController::r2_pressed() {
    return this->r2;
}

unsigned char ReplayController::get_digital_speed() {
    return this->digitalSpeed;
}

void ReplayController::set_digital_speed(unsigned char speed) {
    this->digitalSpeed = speed;
}

void ReplayController::set_line(unsigned char line, unsigned char col, const char* str) {}
void ReplayController::clear_line(unsigned char line) {}
void ReplayController::rumble(const char* str) {}

double ReplayController::left_stick_x() {
    return this->leftStickX;
}

double ReplayController::left_stick_y() {
    return this->leftStickY;
}

double ReplayController::right_stick_x() {
    return this->rightStickX;
}

double ReplayController::right_stick_y() {
    return this->rightStickY;
}

double ReplayController::prev_left_stick_x() {
    return this->prevLeftStickX;
}

double ReplayController::prev_left_stick_y() {
    return this->prevLeftStickY;
}

double ReplayController::prev_right_stick_x() {
    return this->prevRightStickX;
}

double ReplayController::prev_right_stick_y() {
    return this->prevRightStickY;
}

void ReplayController::update() {
    if (this->inf.peek() == std::char_traits<char>::eof()) {
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
                printf("Invalid code! %c", c);
                inf.close();
                return;
        }
    }
    unsigned long long value;

    inf >> value;
    std::memcpy(&leftStickX, &value, sizeof(value));
    inf >> value;
    std::memcpy(&leftStickY, &value, sizeof(value));
    inf >> value;
    std::memcpy(&rightStickX, &value, sizeof(value));
    inf >> value;
    std::memcpy(&rightStickY, &value, sizeof(value));
}

void ReplayController::reset() {
}

void ReplayController::stop() {
    inf.close();
}

std::string ReplayController::describe() {
    return std::string("Replay\n")
            .append("type: ").append(std::to_string(this->type))
            .append("\na: ").append(std::to_string(this->a))
            .append(", b: ").append(std::to_string(this->b))
            .append(", x: ").append(std::to_string(this->x))
            .append(", y: ").append(std::to_string(this->y))
            .append("\nup: ").append(std::to_string(this->up))
            .append(", down: ").append(std::to_string(this->down))
            .append(", left: ").append(std::to_string(this->left))
            .append(", right: ").append(std::to_string(this->right))
            .append("\nl1: ").append(std::to_string(this->l1))
            .append(", l2: ").append(std::to_string(this->l2))
            .append(", r1: ").append(std::to_string(this->r1))
            .append(", r2: ").append(std::to_string(this->r2))
            .append("\nleft stick x: ").append(std::to_string(this->leftStickX))
            .append(", left stick y: ").append(std::to_string(this->leftStickY))
            .append("\nright stick x: ").append(std::to_string(this->rightStickX))
            .append(", right stick y: ").append(std::to_string(this->rightStickY))
            .append("\nprev left stick x: ").append(std::to_string(this->prevLeftStickX))
            .append(", prev left stick y: ").append(std::to_string(this->prevLeftStickY))
            .append("\nprev right stick x: ").append(std::to_string(this->prevRightStickX))
            .append(", prev right stick y: ").append(std::to_string(this->prevRightStickY));
}
