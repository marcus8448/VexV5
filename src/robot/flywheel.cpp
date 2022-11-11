#include "robot/flywheel.hpp"
#include "logger.hpp"

namespace robot {
Flywheel::Flywheel(uint8_t port, uint8_t secondary_port)
    : motor(device::Motor(port, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST, false)),
      secondary_motor(device::Motor(secondary_port, pros::E_MOTOR_GEAR_GREEN, pros::E_MOTOR_BRAKE_COAST, true)) {}

Flywheel::~Flywheel() = default;

void Flywheel::engage(int32_t flywheelSpeed, bool block) {
  this->motor.move_velocity(flywheelSpeed);
  this->secondary_motor.move_velocity(flywheelSpeed);
  double actual = std::abs(this->motor.get_velocity());
  if (actual > flywheelSpeed) {
    if (std::abs(actual - flywheelSpeed) < 10.0) {
      this->state = State::AT_SPEED;
    } else {
      this->state = State::SPINNING_DOWN;
    }
  } else {
    if (std::abs(actual - flywheelSpeed) < 10.0) {
      this->state = State::AT_SPEED;
    } else {
      this->state = State::SPINNING_UP;
    }
  }

  if (block) {
    this->waitForSpeed();
  }
}

void Flywheel::disengage() {
  this->state = State::IDLE;
  this->motor.stop();
  this->secondary_motor.stop();
}

void Flywheel::update(Controller *controller) {
  if (controller->r1_pressed()) {
    this->engage(controller->flywheel_speed());
  } else if (controller->r2_pressed()) {
    this->disengage();
  }
  if (this->state == SPINNING_UP || this->state == SPINNING_DOWN) {
    if (this->isUpToSpeed()) {
      this->state = State::AT_SPEED;
      controller->rumble("-");
      controller->rumble("");
    }
  }
  if (this->state == State::AT_SPEED) {
    if (std::abs(std::abs(this->motor.get_velocity()) - std::abs(this->motor.get_target_velocity())) > 40.0) {
      this->state = State::SPINNING_UP;
    }
  }
}

[[nodiscard]] const device::Motor &Flywheel::get_motor() const { return this->motor; }

double Flywheel::getVelocity() { return this->motor.get_velocity(); }

bool Flywheel::isUpToSpeed() {
  return std::abs(std::abs(this->motor.get_velocity()) - std::abs(this->motor.get_target_velocity())) < 10.0;
}

void Flywheel::waitForSpeed(int millis_timeout) {
  if (this->state == State::IDLE) {
    logger::error("Waiting for flywheel to speed up while it's off!");
    return;
  }
  millis_timeout /= 50;
  int i = 0;
  int32_t target = std::abs(this->motor.get_target_velocity());
  while (target - std::abs(this->motor.get_velocity()) > 10.0) {
    if (i++ == millis_timeout) {
      break;
    }
    pros::delay(20);
  }
  this->state = State::AT_SPEED;
}
} // namespace robot
