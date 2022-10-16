#include "robot/drivetrain.hpp"
#include "configuration.hpp"
#include "error.hpp"
#include "util.hpp"
#include <cmath>
#include <cstdlib>

namespace robot {
robot::Drivetrain::Drivetrain(pros::Motor *rightFront, pros::Motor *leftFront, pros::Motor *rightBack,
                              pros::Motor *leftBack)
    : rightFront(rightFront), leftFront(leftFront), rightBack(rightBack), leftBack(leftBack) {
  this->rightFront->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  this->rightBack->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  this->leftFront->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  this->leftBack->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}

robot::Drivetrain::~Drivetrain() {
  free(this->rightFront);
  this->rightFront = nullptr;
  free(this->rightBack);
  this->rightBack = nullptr;
  free(this->leftFront);
  this->leftFront = nullptr;
  free(this->leftBack);
  this->leftBack = nullptr;
}

bool robot::Drivetrain::is_offset_within(double distance) const {
  return std::fabs(print_error(this->rightFront->get_position()) -
      print_error(this->rightFront->get_target_position())) < distance &&
         std::fabs(print_error(this->leftFront->get_position()) - print_error(this->leftFront->get_target_position())) <
             distance &&
         std::fabs(print_error(this->rightBack->get_position()) - print_error(this->rightBack->get_target_position())) <
             distance &&
         std::fabs(print_error(this->leftBack->get_position()) - print_error(this->leftBack->get_target_position())) <
             distance;
}

void robot::Drivetrain::move(double right_distance, double left_distance, int32_t max_rpm, bool block) const {
  this->move_right(right_distance, max_rpm);
  this->move_left(left_distance, max_rpm);
  if (block) {
    while (!this->is_offset_within(5.0)) {
      pros::delay(50);
    }
    do {
      pros::delay(25);
    } while (!this->is_offset_within(1.0));
  }
}

void robot::Drivetrain::forwards(double distance, int32_t max_rpm, bool block) {
  this->move(in_to_e_units(distance), in_to_e_units(distance), max_rpm, block);
}

void robot::Drivetrain::backwards(double distance, int32_t max_rpm, bool block) {
  this->move(-in_to_e_units(distance), -in_to_e_units(distance), max_rpm, block);
}

void robot::Drivetrain::turn_right(double degrees, int32_t max_rpm, bool block) {
  this->move(-turn_to_e_units(degrees), turn_to_e_units(degrees), max_rpm, block);
}

void robot::Drivetrain::turn_left(double degrees, int32_t max_rpm, bool block) {
  this->move(turn_to_e_units(degrees), -turn_to_e_units(degrees), max_rpm, block);
}

void robot::Drivetrain::move_right(int32_t voltage) const {
  print_error(this->rightFront->move(voltage));
  print_error(this->rightBack->move(voltage));
}

void robot::Drivetrain::move_left(int32_t voltage) const {
  print_error(this->leftFront->move(voltage));
  print_error(this->leftBack->move(voltage));
}

void robot::Drivetrain::update(Controller *controller) {
  if (config::get_drivetrain_control_scheme() == config::DrivetrainControlScheme::ARCADE_DRIVE) {
    double joystickRotX = controller->right_stick_x();
    double joystickY = controller->left_stick_y();
    double joystickX = controller->left_stick_x();

    this->move_right(static_cast<int32_t>(joystickY - joystickRotX + joystickX));
    this->move_left(static_cast<int32_t>(joystickY + joystickRotX - joystickX));
  } else {
    this->move_right(static_cast<int32_t>(controller->right_stick_y()));
    this->move_left(static_cast<int32_t>(controller->left_stick_y()));
  }
}

void robot::Drivetrain::stop() const {
  this->rightFront->move(0);
  this->leftFront->move(0);
  this->rightBack->move(0);
  this->leftBack->move(0);
}

void robot::Drivetrain::tare() const {
  this->rightFront->tare_position();
  this->leftFront->tare_position();
  this->rightBack->tare_position();
  this->leftBack->tare_position();
}

void Drivetrain::move_right(double distance, int32_t max_rpm) const {
  print_error(this->rightFront->move_relative(distance, max_rpm));
  print_error(this->rightBack->move_relative(distance, max_rpm));
}

void Drivetrain::move_left(double distance, int32_t max_rpm) const {
  print_error(this->leftFront->move_relative(distance, max_rpm));
  print_error(this->leftBack->move_relative(distance, max_rpm));
}
} // namespace robot
