#include "robot/drivetrain.hpp"
#include "configuration.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "pros/error.h"
#include "robot/device/motor.hpp"
#include "util.hpp"
#include <algorithm>
#include <iostream>

#define SAFE_MAX_MOTOR_VOLTAGE 120

namespace robot {
Drivetrain::Drivetrain(const uint8_t rightFront, const uint8_t leftFront, const uint8_t rightBack,
                       const uint8_t leftBack, const uint8_t inertial)
    : rightFront(device::Motor(rightFront, "Right Front", pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_COAST, false)),
      leftFront(device::Motor(leftFront, "Left Front", pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_COAST, true)),
      rightBack(device::Motor(rightBack, "Right Back", pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_COAST, false)),
      leftBack(device::Motor(leftBack, "Left Back", pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_COAST, true)),
      imu(device::Inertial(inertial, "IDrive")) {}

Drivetrain::~Drivetrain() = default;

bool Drivetrain::is_at_target() const {
  return this->rightFront.is_at_target() && this->leftFront.is_at_target() && this->rightBack.is_at_target() &&
         this->leftBack.is_at_target();
}

void Drivetrain::move(double right_distance, double left_distance, int16_t max_rpm) {
  this->move_right_distance(right_distance, max_rpm);
  this->move_left_distance(left_distance, max_rpm);
}

void Drivetrain::forwards(double distance, int16_t max_rpm, uint16_t timeout_millis) {
  this->move(util::in_to_e_units(distance), util::in_to_e_units(distance), max_rpm);
  this->await_move(timeout_millis);
}

void Drivetrain::backwards(double distance, int16_t max_rpm, uint16_t timeout_millis) {
  this->move(-util::in_to_e_units(distance), -util::in_to_e_units(distance), max_rpm);
  this->await_move(timeout_millis);
}

void Drivetrain::turn_right(double degrees, int16_t max_rpm, uint16_t timeout_millis) {
  this->targetRotation += degrees;
  double rotation = this->imu.get_heading();
  if (this->imu.is_connected() && (rotation > -1e10 && rotation < 1e10)) {
    info("supposedly targeting %f, actual %f, target: %f", degrees, (this->targetRotation - rotation),
         this->targetRotation);
    info("rotation %f maybe %f, %f", (double)rotation, (double)(this->targetRotation + rotation),
         (int)(-this->targetRotation - rotation));
    std::cout << rotation << ' ' << (double)(this->targetRotation + rotation) << ' '
              << (int)(-this->targetRotation - rotation) << std::endl;

    degrees = this->targetRotation - rotation;
    if (degrees > 180) {
      degrees -= 360;
    } else if (degrees < -180) {
      degrees += 360;
    }
  } else {
    if (rotation == PROS_ERR_F) {
      check_error();
    }
    warn("IMU not connected/error");
  }
  double distance = util::turn_to_e_units(degrees);
  this->move(-distance, distance, max_rpm);
  this->await_move(timeout_millis);
}

void Drivetrain::turn_left(double degrees, int16_t max_rpm, uint16_t timeout_millis) {
  this->turn_right(-degrees, max_rpm, timeout_millis);
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
  if (!this->is_at_target()) {
    warn("Drivetrain move timed out. %f / %f, %f / %f | %f / %f, %f / %f", this->leftFront.get_position(),
         this->leftFront.get_target_position(), this->leftBack.get_position(), this->leftBack.get_target_position(),
         this->rightFront.get_position(), this->rightFront.get_target_position(), this->rightBack.get_position(),
         this->rightBack.get_target_position());
  }
}

void Drivetrain::update(Controller *controller) {
  if (controller->down_pressed() == 1) {
    this->arcade = !this->arcade;
    controller->rumble("-");
  }

  double left;
  double right;
  if (this->arcade) {
    double joystickRotX = controller->right_stick_x();
    double joystickY = controller->left_stick_y();
    left = joystickY + joystickRotX;
    right = joystickY - joystickRotX;

    if (right > SAFE_MAX_MOTOR_VOLTAGE || left > SAFE_MAX_MOTOR_VOLTAGE) {
      double max = std::max(right, left);
      right = (right / max) * SAFE_MAX_MOTOR_VOLTAGE;
      left = (left / max) * SAFE_MAX_MOTOR_VOLTAGE;
    }
  } else {
    right = controller->right_stick_y();
    left = controller->left_stick_y();
  }
  if (right == 0.0 && left == 0.0) {
    if (++timeOff == 1000) {
      this->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
      this->brake();
    }
  } else {
    timeOff = 0;
    this->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  }
  this->power_right(right * 100.0 / 127.0);
  this->power_left(left * 100.0 / 127.0);
}

void Drivetrain::brake() {
  this->rightFront.brake();
  this->leftFront.brake();
  this->rightBack.brake();
  this->leftBack.brake();
}

void Drivetrain::set_brake_mode(pros::motor_brake_mode_e brake_mode) {
  this->leftFront.set_brake_mode(brake_mode);
  this->leftBack.set_brake_mode(brake_mode);
  this->rightFront.set_brake_mode(brake_mode);
  this->rightBack.set_brake_mode(brake_mode);
}

void Drivetrain::tare() {
  this->rightFront.tare();
  this->leftFront.tare();
  this->rightBack.tare();
  this->leftBack.tare();
  this->targetRotation = 0.0;
  this->imu.tare();
}

void Drivetrain::move_right_distance(double distance, int16_t max_rpm) {
  this->rightFront.move_relative(distance, max_rpm);
  this->rightBack.move_relative(distance, max_rpm);
}

void Drivetrain::move_left_distance(double distance, int16_t max_rpm) {
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

void Drivetrain::power(double percent) {
  this->power_right(percent);
  this->power_left(percent);
}
} // namespace robot
