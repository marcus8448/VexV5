#include "drivetrain.hpp"
#include "error.hpp"
#include "util.hpp"
#include <cmath>

Drivetrain::Drivetrain(pros::Motor *rightFront, pros::Motor *leftFront, pros::Motor *rightBack, pros::Motor *leftBack)
    : rightFront(rightFront), leftFront(leftFront), rightBack(rightBack), leftBack(leftBack) {
  this->rightFront->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  this->rightBack->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  this->leftFront->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  this->leftBack->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}

Drivetrain::~Drivetrain() {
  // delete this->rightFront;
  // delete this->rightBack;
  // delete this->leftFront;
  // delete this->leftBack;
}

bool Drivetrain::is_offset_within(double distance) {
  return std::fabs(check_error(this->rightFront->get_position()) - check_error(this->rightFront->get_target_position())) <
      distance &&
      std::fabs(check_error(this->leftFront->get_position()) - check_error(this->leftFront->get_target_position())) <
          distance &&
      std::fabs(check_error(this->rightBack->get_position()) - check_error(this->rightBack->get_target_position())) <
          distance &&
      std::fabs(check_error(this->leftBack->get_position()) - check_error(this->leftBack->get_target_position())) <
          distance;
}

void Drivetrain::move_for(double right_distance, double left_distance, int32_t max_rpm, bool block) {
  check_error(this->rightFront->move_relative(right_distance, max_rpm));
  check_error(this->rightBack->move_relative(right_distance, max_rpm));
  check_error(this->leftFront->move_relative(left_distance, max_rpm));
  check_error(this->leftBack->move_relative(left_distance, max_rpm));
  if (block) {
    while (!this->is_offset_within(5.0)) {
      pros::delay(50);
    }
    do {
      pros::delay(25);
    } while (!this->is_offset_within(1.0));
  }
}

void Drivetrain::forwards(double distance, int32_t max_rpm, bool block) {
  this->move_for(in_to_rot(distance), in_to_rot(distance), max_rpm, block);
}

void Drivetrain::backwards(double distance, int32_t max_rpm, bool block) {
  this->move_for(-in_to_rot(distance), -in_to_rot(distance), max_rpm, block);
}

void Drivetrain::turn_right(double angle, int32_t max_rpm, bool block) {
  this->move_for(-turn_to_rot(angle), turn_to_rot(angle), max_rpm, block);
}

void Drivetrain::turn_left(double angle, int32_t max_rpm, bool block) {
  this->move_for(turn_to_rot(angle), -turn_to_rot(angle), max_rpm, block);
}

void Drivetrain::move_right(int32_t voltage) {
  check_error(this->rightFront->move(voltage));
  check_error(this->rightBack->move(voltage));
}

void Drivetrain::move_left(int32_t voltage) {
  check_error(this->leftFront->move(voltage));
  check_error(this->leftBack->move(voltage));
}

void Drivetrain::update(Controller *controller) {
  this->move_right((int) controller->right_stick_y());
  this->move_left((int) controller->left_stick_y());
}

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
