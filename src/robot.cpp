#include "robot.hpp"
#include "error.hpp"
#include "util.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include <cmath>
#include <iostream>

double DualPositioned::get_position() {
    return (this->get_left_position() + this->get_right_position()) / 2.0;
}

double DualPositioned::get_target_position() {
    return (this->get_left_target_position() + this->get_right_target_position()) / 2.0;
}

unsigned short int OpController::a_pressed() {
    return this->a;
}

unsigned short int OpController::b_pressed() {
    return this->b;
}

unsigned short int OpController::x_pressed() {
    return this->x;
}

unsigned short int OpController::y_pressed() {
    return this->y;
}

unsigned short int OpController::up_pressed() {
    return this->up;
}

unsigned short int OpController::down_pressed() {
    return this->down;
}

unsigned short int OpController::left_pressed() {
    return this->left;
}

unsigned short int OpController::right_pressed() {
    return this->right;
}

unsigned short int OpController::l1_pressed() {
    return this->l1;
}

unsigned short int OpController::l2_pressed() {
    return this->l2;
}

unsigned short int OpController::r1_pressed() {
    return this->r1;
}

unsigned short int OpController::r2_pressed() {
    return this->r2;
}

double OpController::left_stick_x() {
    return this->leftStickX;
}

double OpController::left_stick_y() {
    return this->leftStickY;
}

double OpController::right_stick_x() {
    return this->rightStickX;
}

double OpController::right_stick_y() {
    return this->rightStickY;
}

double OpController::prev_left_stick_x() {
    return this->prevLeftStickX;
}

double OpController::prev_left_stick_y() {
    return this->prevLeftStickY;
}

double OpController::prev_right_stick_x() {
    return this->prevRightStickX;
}

double OpController::prev_right_stick_y() {
    return this->prevRightStickY;
}

unsigned char OpController::get_digital_speed() {
    return this->digitalSpeed;
}

void OpController::set_digital_speed(unsigned char speed) {
    this->digitalSpeed = speed;
}

void OpController::set_line(unsigned char line, unsigned char col, const char* str) {
    erri(this->controller.set_text(line, col, str));
}

void OpController::clear_line(unsigned char line) {
    erri(this->controller.clear_line(line));
}

void OpController::rumble(const char* str) {
    erri(this->controller.rumble(str));
}

void OpController::update() {
    if (erri(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))) {
        this->a++;
    } else {
        this->a = 0;
    }

    if (erri(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))) {
        this->b++;
    } else {
        this->b = 0;
    }

    if (erri(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))) {
        this->x++;
    } else {
        this->x = 0;
    }

    if (erri(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y))) {
        this->y++;
    } else {
        this->y = 0;
    }

    if (erri(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP))) {
        this->up++;
    } else {
        this->up = 0;
    }

    if (erri(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))) {
        this->down++;
    } else {
        this->down = 0;
    }

    if (erri(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))) {
        this->left++;
    } else {
        this->left = 0;
    }

    if (erri(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))) {
        this->right++;
    } else {
        this->right = 0;
    }

    if (erri(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))) {
        this->l1++;
    } else {
        this->l1 = 0;
    }

    if (erri(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))) {
        this->l2++;
    } else {
        this->l2 = 0;
    }

    if (erri(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))) {
        this->r1++;
    } else {
        this->r1 = 0;
    }

    if (erri(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))) {
        this->r2++;
    } else {
        this->r2 = 0;
    }

    this->prevLeftStickX = this->leftStickX;
    this->leftStickX = errd(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X));

    this->prevLeftStickY = this->leftStickY;
    this->leftStickY = errd(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));

    this->prevRightStickX = this->rightStickX;
    this->rightStickX = errd(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));

    this->prevRightStickY = this->rightStickY;
    this->rightStickY = errd(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));

    if (this->up_pressed()) {
        this->set_digital_speed(std::min(this->get_digital_speed() + 1, 127));
    } else if (this->down_pressed()) {
        this->set_digital_speed(std::max(this->get_digital_speed() - 1, 127));
    }

    if (this->up_pressed() || this->down_pressed()) {
        this->set_line(0, 0, ("Dig Spd: " + std::to_string(this->get_digital_speed()).append(" ")).c_str());//append ' ' to clear out buffer
    }
}

void OpController::stop() {
}

void OpController::reset() {
    this->a = 0;
    this->b = 0;
    this->x = 0;
    this->y = 0;
    this->up = 0;
    this->down = 0;
    this->left = 0;
    this->right = 0;
    this->l1 = 0;
    this->l2 = 0;
    this->r1 = 0;
    this->r2 = 0;
    this->leftStickX = 0;
    this->leftStickY = 0;
    this->rightStickX = 0;
    this->rightStickY = 0;
    this->prevLeftStickX = 0;
    this->prevLeftStickY = 0;
    this->prevRightStickX = 0;
    this->prevRightStickY = 0;
}

OpController::OpController(pros::Controller controller) : controller(controller) {
}

std::string OpController::describe() {
    return std::string("Op\n")
            .append("a: ").append(std::to_string(this->a))
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

Drivetrain::Drivetrain(pros::Motor* rightFront, pros::Motor* leftFront, pros::Motor* rightBack, pros::Motor* leftBack) : rightFront(rightFront), leftFront(leftFront), rightBack(rightBack), leftBack(leftBack) {
    this->rightFront->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    this->rightBack->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    this->leftFront->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    this->leftBack->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}

bool Drivetrain::is_offset_within(double distance) {
    return fabs(errd(this->rightFront->get_position()) - errd(this->rightFront->get_target_position())) < distance && fabs(errd(this->leftFront->get_position()) - errd(this->leftFront->get_target_position())) < distance && fabs(errd(this->rightBack->get_position()) - errd(this->rightBack->get_target_position())) < distance && fabs(errd(this->leftBack->get_position()) - errd(this->leftBack->get_target_position())) < distance;
}

void Drivetrain::move_for(double right_distance, double left_distance, int max_rpm, bool block) {
    erri(this->rightFront->move_relative(right_distance, max_rpm));
    erri(this->rightBack->move_relative(right_distance, max_rpm));
    erri(this->leftFront->move_relative(left_distance, max_rpm));
    erri(this->leftBack->move_relative(left_distance, max_rpm));
    if (block) {
        while (!this->is_offset_within(5.0)) {
            pros::delay(50);
        }
        do {
            pros::delay(25);
        } while (!this->is_offset_within(1.0));
    }
}

void Drivetrain::forwards(double distance, int max_rpm, bool block) {
    this->move_for(in_to_rot(distance), in_to_rot(distance), max_rpm, block);
}

void Drivetrain::backwards(double distance, int max_rpm, bool block) {
    this->move_for(-in_to_rot(distance), -in_to_rot(distance), max_rpm, block);
}

void Drivetrain::turn_right(double angle, int max_rpm, bool block) {
    this->move_for(-turn_to_rot(angle), turn_to_rot(angle), max_rpm, block);
}

void Drivetrain::turn_left(double angle, int max_rpm, bool block) {
    this->move_for(turn_to_rot(angle), -turn_to_rot(angle), max_rpm, block);
}

void Drivetrain::move_right(int voltage) {
    erri(this->rightFront->move(voltage));
    erri(this->rightBack->move(voltage));
}

void Drivetrain::move_left(int voltage) {
    erri(this->leftFront->move(voltage));
    erri(this->leftBack->move(voltage));
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
void Drivetrain::update(Controller* controller) {
    this->move_right(controller->right_stick_y());
    this->move_left(controller->left_stick_y());
}
#pragma clang diagnostic pop

void Drivetrain::stop() {
    this->rightFront->move(0);
    this->leftFront->move(0);
    this->rightBack->move(0);
    this->leftBack->move(0);
}

void Drivetrain::reset() {
    this->rightFront->tare_position();
    this->leftFront->tare_position();
    this->rightBack->tare_position();
    this->leftBack->tare_position();
}

std::string Drivetrain::describe() {
    return std::string("Right Front: ").append(describe_motor(this->rightFront))
                .append("\nLeft Front: ").append(describe_motor(this->leftFront))
                .append("\nRight Back: ").append(describe_motor(this->rightBack))
                .append("\nLeft Back: ").append(describe_motor(this->leftBack));
}

Robot::Robot(Drivetrain* drivetrain) : drivetrain(drivetrain), controller(nullptr) {
}

void Robot::update() {
    if (this->controller == nullptr) {
        println("Controller is null?");
    }
    if (this->drivetrain == nullptr) {
        println("Drivetrain is null?");
    }
    this->controller->update();
    this->drivetrain->update(this->controller);
}

void Robot::reset() {
    this->controller->reset();
    this->drivetrain->reset();
}

void Robot::stop() {
    this->controller->stop();
    this->drivetrain->stop();
}

Robot::~Robot() {
    println("Robot destructor called");
    if (controller != nullptr) {
        delete controller;
        controller = nullptr;
    }
}

std::string Robot::describe() {
    return std::string("\nController: ").append(indent(this->controller->describe())).append("\nDrivetrain: ").append(indent(this->drivetrain->describe()));
}
