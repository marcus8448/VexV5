#include <cmath>
#include "vexv5/debug.hpp"
#include "vexv5/robot.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"

#define PI 3.14159265358979323846
#define WHEEL_SIZE 2.0625
#define WHEEL_CIRCUMFRENCE (WHEEL_SIZE * 2.0 * PI)

double in_to_rot(double inches) {
    return inches / WHEEL_CIRCUMFRENCE * 360.0;
}

double turn_to_rot(double degrees) {
    return degrees * 4.0;
}

double DualPositioned::get_position() {
    return (this->get_left_position() + this->get_right_position()) / 2.0;
}

double DualPositioned::get_target_position() {
    return (this->get_left_target_position() + this->get_right_target_position()) / 2.0;
}

bool OpController::a_pressed() {
    return this->a;
}

bool OpController::b_pressed() {
    return this->b;
}

bool OpController::x_pressed() {
    return this->x;
}

bool OpController::y_pressed() {
    return this->y;
}

bool OpController::up_pressed() {
    return this->up;
}

bool OpController::down_pressed() {
    return this->down;
}

bool OpController::left_pressed() {
    return this->left;
}

bool OpController::right_pressed() {
    return this->right;
}

bool OpController::l1_pressed() {
    return this->l1;
}

bool OpController::l2_pressed() {
    return this->l2;
}

bool OpController::r1_pressed() {
    return this->r1;
}

bool OpController::r2_pressed() {
    return this->r2;
}

unsigned short int OpController::a_press_length() {
    return this->a;
}

unsigned short int OpController::b_press_length() {
    return this->b;
}

unsigned short int OpController::x_press_length() {
    return this->x;
}

unsigned short int OpController::y_press_length() {
    return this->y;
}

unsigned short int OpController::up_press_length() {
    return this->up;
}

unsigned short int OpController::down_press_length() {
    return this->down;
}

unsigned short int OpController::left_press_length() {
    return this->left;
}

unsigned short int OpController::right_press_length() {
    return this->right;
}

unsigned short int OpController::l1_press_length() {
    return this->l1;
}

unsigned short int OpController::l2_press_length() {
    return this->l2;
}

unsigned short int OpController::r1_press_length() {
    return this->r1;
}

unsigned short int OpController::r2_press_length() {
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

void OpController::set_digital_speed(unsigned char digitalSpeed) {
    this->digitalSpeed = digitalSpeed;
}

void OpController::set_line(unsigned char line, unsigned char col, const char* str) {
    this->controller.set_text(line, col, str);
}

void OpController::clear_line(unsigned char line) {
    this->controller.clear_line(line);
}

void OpController::rumble(const char* str) {
    this->controller.rumble(str);
}

void OpController::update() {
    if (this->up_pressed()) {
        this->set_digital_speed(std::min(this->get_digital_speed() + 1, 127));
    } else if (this->down_pressed()) {
        this->set_digital_speed(std::max(this->get_digital_speed() - 1, 127));
    }

    if (this->up_pressed() || this->down_pressed()) {
        this->set_line(0, 0, ("Dig Spd: " + std::to_string(this->get_digital_speed()).append(" ")).c_str()); //append ' ' to clear out buffer
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

Drivetrain::Drivetrain(pros::Motor rightFront, pros::Motor leftFront, pros::Motor rightBack, pros::Motor leftBack): rightFront(rightFront), leftFront(leftFront), rightBack(rightBack), leftBack(leftBack)  {
    this->rightFront.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    this->rightBack.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    this->leftFront.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    this->leftBack.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
};

bool Drivetrain::is_offset_within(double distance) {
    return fabs(errd(this->rightFront.get_position()) - errd(this->rightFront.get_target_position())) < distance
        && fabs(errd(this->leftFront.get_position()) - errd(this->leftFront.get_target_position())) < distance
        && fabs(errd(this->rightBack.get_position()) - errd(this->rightBack.get_target_position())) < distance
        && fabs(errd(this->leftBack.get_position()) - errd(this->leftBack.get_target_position())) < distance;
}

void Drivetrain::move_for(double right_distance, double left_distance, int max_rpm = 60, bool block = true) {
    erri(this->rightFront.move_relative(right_distance, max_rpm));
    erri(this->rightBack.move_relative(right_distance, max_rpm));
    erri(this->leftFront.move_relative(left_distance, max_rpm));
    erri(this->leftBack.move_relative(left_distance, max_rpm));
    if (block) {
        double inv_progress = 1.0;
        while (!this->is_offset_within(5.0)) {
            pros::delay(50);
        }
        do {
            pros::delay(25);
        } while (!this->is_offset_within(1.0));
    }
}

void Drivetrain::forwards(double distance, int max_rpm = 100, bool block = true) {
    this->move_for(in_to_rot(distance), in_to_rot(distance), max_rpm, block);
}

void Drivetrain::backwards(double distance, int max_rpm = 100, bool block = true) {
    this->move_for(-in_to_rot(distance), -in_to_rot(distance), max_rpm, block);
}

void Drivetrain::turn_right(double angle, int max_rpm = 100, bool block = true) {
    this->move_for(-turn_to_rot(angle), turn_to_rot(angle), max_rpm, block);
}

void Drivetrain::turn_left(double angle, int max_rpm = 100, bool block = true) {
    this->move_for(turn_to_rot(angle), -turn_to_rot(angle), max_rpm);
}

void Drivetrain::move_right(int voltage) {
    erri(this->rightFront.move(voltage));
    erri(this->rightBack.move(voltage));
}

void Drivetrain::move_left(int voltage) {
    erri(this->leftFront.move(voltage));
    erri(this->leftBack.move(voltage));
}

void Drivetrain::update(Controller controller) {
    this->move_right(controller.right_stick_y());
    this->move_left(controller.left_stick_y());
}

void Drivetrain::stop() {
    this->rightFront.move(0);
    this->leftFront.move(0);
    this->rightBack.move(0);
    this->leftBack.move(0);
}

void Drivetrain::reset() {
    this->rightFront.tare_position();
    this->leftFront.tare_position();
    this->rightBack.tare_position();
    this->leftBack.tare_position();
}

Robot::Robot(): drivetrain(Drivetrain(pros::Motor(10, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES), pros::Motor(1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES), pros::Motor(20, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES), pros::Motor(11, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES))), controller(OpController(pros::Controller(pros::E_CONTROLLER_MASTER))) {
    this->stop();
};

Robot::Robot(Drivetrain drivetrain, Controller controller): drivetrain(drivetrain), controller(controller) {
    this->stop();
};

void Robot::forwards(double distance, int max_rpm = 100, bool block = true) {
    this->drivetrain.forwards(distance, max_rpm, block);
}

void Robot::backwards(double distance, int max_rpm = 100, bool block = true) {
    this->drivetrain.backwards(distance, max_rpm, block);
}

void Robot::turn_right(double angle, int max_rpm = 100, bool block = true) {
    this->drivetrain.turn_right(angle, max_rpm, block);
}

void Robot::turn_left(double angle, int max_rpm = 100, bool block = true) {
    this->drivetrain.turn_left(angle, max_rpm, block);
}

void Robot::update() {
    this->controller.update();
    this->drivetrain.update(this->controller);
}

void Robot::reset() {
    this->controller.reset();
    this->drivetrain.reset();
}

void Robot::stop() {
    this->controller.stop();
    this->drivetrain.stop();
}
