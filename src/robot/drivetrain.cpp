#include "configuration.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "util.hpp"

#include "robot/device/motor.hpp"
#include "robot/drivetrain.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>

#define SAFE_MAX_MOTOR_VOLTAGE 125

namespace robot {
Drivetrain::Drivetrain(const uint8_t rightFront, const uint8_t leftFront, const uint8_t rightBack,
                       const uint8_t leftBack)
    : rightFront(device::Motor(rightFront, pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_BRAKE, false)),
      leftFront(device::Motor(leftFront, pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_BRAKE, true)),
      rightBack(device::Motor(rightBack, pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_BRAKE, false)),
      leftBack(device::Motor(leftBack, pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_BRAKE, true)) {}

Drivetrain::~Drivetrain() = default;

bool Drivetrain::is_at_target(double distance) const {
  return this->rightFront.is_at_target() && this->leftFront.is_at_target() && this->rightBack.is_at_target() &&
         this->leftBack.is_at_target();
}

void Drivetrain::move(double right_distance, double left_distance, int32_t max_rpm, bool block) {
  this->move_right_distance(right_distance, max_rpm);
  this->move_left_distance(left_distance, max_rpm);
  if (block) {
    while (!this->is_at_target(5.0)) {
      pros::delay(50);
    }
    pros::delay(25);
  }
}

void Drivetrain::forwards(double distance, int32_t max_rpm, bool block) {
  this->move(in_to_e_units(distance), in_to_e_units(distance), max_rpm, block);
}

void Drivetrain::backwards(double distance, int32_t max_rpm, bool block) {
  this->move(-in_to_e_units(distance), -in_to_e_units(distance), max_rpm, block);
}

void Drivetrain::turn_right(double degrees, int32_t max_rpm, bool block) {
  this->move(-turn_to_e_units(degrees), turn_to_e_units(degrees), max_rpm, block);
}

void Drivetrain::turn_left(double degrees, int32_t max_rpm, bool block) {
  this->move(turn_to_e_units(degrees), -turn_to_e_units(degrees), max_rpm, block);
}

void Drivetrain::move_right(double percent) {
  this->rightFront.move_percentage(percent);
  this->rightBack.move_percentage(percent);
}

void Drivetrain::move_left(double percent) {
  this->leftFront.move_percentage(percent);
  this->leftBack.move_percentage(percent);
}

void Drivetrain::update(Controller *controller) {
  if (config::get_instance()->get_drivetrain_control_scheme() == config::DrivetrainControlScheme::ARCADE_DRIVE) {
    double joystickRotX = controller->right_stick_x();
    double joystickY = controller->left_stick_y();
    double right = joystickY - joystickRotX;
    double left = joystickY + joystickRotX;
    int32_t rOver = 0;
    int32_t lOver = 0;
    if (right < -SAFE_MAX_MOTOR_VOLTAGE) {
      rOver = right + SAFE_MAX_MOTOR_VOLTAGE;
    } else if (right > SAFE_MAX_MOTOR_VOLTAGE) {
      rOver = right - SAFE_MAX_MOTOR_VOLTAGE;
    }

    if (left < -SAFE_MAX_MOTOR_VOLTAGE) {
      lOver = left + SAFE_MAX_MOTOR_VOLTAGE;
    } else if (left > SAFE_MAX_MOTOR_VOLTAGE) {
      lOver = left - SAFE_MAX_MOTOR_VOLTAGE;
    }
    int32_t over = std::max(rOver, lOver);
    if (over > 0) {
      if (left > 0) {
        left -= over;
      } else {
        left += over;
      }
      if (right > 0) {
        right -= over;
      } else {
        right += over;
      }
    }

    this->move_right(right / 127.0);
    this->move_left(left / 127.0);
  } else {
    this->move_right(controller->right_stick_y() / 127.0);
    this->move_left(controller->left_stick_y() / 127.0);
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
  this->rightFront.move_realative_target(distance, max_rpm);
  this->rightBack.move_realative_target(distance, max_rpm);
}

void Drivetrain::move_left_distance(double distance, int32_t max_rpm) {
  this->leftFront.move_realative_target(distance, max_rpm);
  this->leftBack.move_realative_target(distance, max_rpm);
}
} // namespace robot
