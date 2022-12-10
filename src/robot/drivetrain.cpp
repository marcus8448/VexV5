#include "robot/drivetrain.hpp"
#include "configuration.hpp"
#include "logger.hpp"
#include "robot/device/motor.hpp"
#include "util.hpp"
#include <algorithm>

#define SAFE_MAX_MOTOR_VOLTAGE 120

namespace robot {
Drivetrain::Drivetrain(const uint8_t rightFront, const uint8_t leftFront, const uint8_t rightBack,
                       const uint8_t leftBack)
    : rightFront(device::Motor(rightFront, "Right Front", pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_BRAKE, false)),
      leftFront(device::Motor(leftFront, "Left Front", pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_BRAKE, true)),
      rightBack(device::Motor(rightBack, "Right Back", pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_BRAKE, false)),
      leftBack(device::Motor(leftBack, "Left Back", pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_BRAKE, true)) {}

Drivetrain::~Drivetrain() = default;

bool Drivetrain::is_at_target() const {
  return this->rightFront.is_at_target() && this->leftFront.is_at_target() && this->rightBack.is_at_target() &&
         this->leftBack.is_at_target();
}

void Drivetrain::move(double right_distance, double left_distance, int32_t max_rpm) {
  this->move_right_distance(right_distance, max_rpm);
  this->move_left_distance(left_distance, max_rpm);
}

void Drivetrain::forwards(double distance, int32_t max_rpm, uint16_t timeout_millis) {
  this->move(in_to_e_units(distance), in_to_e_units(distance), max_rpm);
  this->await_move(timeout_millis);
}

void Drivetrain::backwards(double distance, int32_t max_rpm, uint16_t timeout_millis) {
  this->move(-in_to_e_units(distance), -in_to_e_units(distance), max_rpm);
  this->await_move(timeout_millis);
}

void Drivetrain::turn_right(double degrees, int32_t max_rpm, uint16_t timeout_millis) {
  this->move(-turn_to_e_units(degrees), turn_to_e_units(degrees), max_rpm);
  this->await_move(timeout_millis);
}

void Drivetrain::turn_left(double degrees, int32_t max_rpm, uint16_t timeout_millis) {
  this->move(turn_to_e_units(degrees), -turn_to_e_units(degrees), max_rpm);
  this->await_move(timeout_millis);
}

void Drivetrain::await_move(uint16_t timeout_millis) const {
  if (timeout_millis == 0)
    return;
  timeout_millis = static_cast<uint16_t>(timeout_millis / 50.0);
  for (uint16_t i = 0; i < timeout_millis; i++) {
    if (this->is_at_target())
      break;
    pros::delay(50);
  }
}

void Drivetrain::update(Controller *controller) {
  if (config::get_instance()->get_control_scheme() == config::DrivetrainControlScheme::ARCADE) {
    double joystickRotX = controller->right_stick_x();
    double joystickY = controller->left_stick_y();
    double right = joystickY - joystickRotX;
    double left = joystickY + joystickRotX;

    if (right > SAFE_MAX_MOTOR_VOLTAGE || left > SAFE_MAX_MOTOR_VOLTAGE) {
      double max = std::max(right, left);
      right = (right / max) * SAFE_MAX_MOTOR_VOLTAGE;
      left = (left / max) * SAFE_MAX_MOTOR_VOLTAGE;
    }

    this->power_right(right / 127.0 * 100.0);
    this->power_left(left / 127.0 * 100.0);
  } else {
    this->power_right(controller->right_stick_y() / 127.0 * 100.0);
    this->power_left(controller->left_stick_y() / 127.0 * 100.0);
  }
}

void Drivetrain::stop() {
  this->rightFront.stop();
  this->leftFront.stop();
  this->rightBack.stop();
  this->leftBack.stop();
}

void Drivetrain::tare() {
  this->rightFront.tare();
  this->leftFront.tare();
  this->rightBack.tare();
  this->leftBack.tare();
}

void Drivetrain::move_right_distance(double distance, int32_t max_rpm) {
  this->rightFront.move_relative(distance, max_rpm);
  this->rightBack.move_relative(distance, max_rpm);
}

void Drivetrain::move_left_distance(double distance, int32_t max_rpm) {
  this->leftFront.move_relative(distance, max_rpm);
  this->leftBack.move_relative(distance, max_rpm);
}

void Drivetrain::power_right(double percent) {
  this->rightFront.move_percentage(percent);
  this->rightBack.move_percentage(percent);
}

void Drivetrain::power_left(double percent) {
  this->leftFront.move_percentage(percent);
  this->leftBack.move_percentage(percent);
}
} // namespace robot
