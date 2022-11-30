#include "robot/flywheel.hpp"
#include "logger.hpp"

namespace robot {
Flywheel::Flywheel(uint8_t port, uint8_t secondary_port)
    : first_motor(device::Motor(port, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST, false)), second_motor(device::Motor(secondary_port, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST, false)) {}

Flywheel::~Flywheel() = default;

void Flywheel::engage(int32_t flywheelSpeed, bool block) {
  this->first_motor.move_velocity(flywheelSpeed);
  this->second_motor.move_velocity(flywheelSpeed);
  double actual = std::abs(this->first_motor.get_velocity());
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
    this->wait_for_speed();
  }
}

void Flywheel::disengage() {
  this->state = State::IDLE;
  this->first_motor.stop();
  this->second_motor.stop();
}

void Flywheel::update(Controller *controller) {
  if (controller->r1_pressed()) {
    this->engage(controller->flywheel_speed());
  } else if (controller->x_pressed()) {
    this->disengage();
  }
  if (this->state == SPINNING_UP || this->state == SPINNING_DOWN) {
    if (this->is_up_to_speed()) {
      this->state = State::AT_SPEED;
      controller->rumble("-");
    }
  }
  if (this->state == State::AT_SPEED) {
    if (std::abs(std::abs(this->first_motor.get_velocity()) - std::abs(this->first_motor.get_target_velocity())) > 40.0) {
      this->state = State::SPINNING_UP;
    }
  }
}

[[nodiscard]] const device::Motor &Flywheel::get_first_motor() const { return this->first_motor; }
[[nodiscard]] const device::Motor &Flywheel::get_second_motor() const { return this->second_motor; }

double Flywheel::get_first_motor_velocity() { return this->first_motor.get_velocity(); }
double Flywheel::get_second_motor_velocity() { return this->second_motor.get_velocity(); }

bool Flywheel::is_up_to_speed() {
  return std::abs(std::abs(this->first_motor.get_velocity()) - std::abs(this->first_motor.get_target_velocity())) < 10.0
  && std::abs(std::abs(this->second_motor.get_velocity()) - std::abs(this->second_motor.get_target_velocity())) < 10.0;
}

void Flywheel::wait_for_speed(uint16_t millis_timeout) {
  if (this->state == State::IDLE) {
    logger::error("Waiting for flywheel to speed up while it's off!");
    return;
  }
  millis_timeout /= 50;
  int16_t i = 0;
  int32_t target = std::abs(this->first_motor.get_target_velocity());
  while (target - std::abs(this->first_motor.get_velocity()) > 10.0 || target - std::abs(this->second_motor.get_velocity()) > 10.0) {
    if (i++ == millis_timeout) {
      break;
    }
    pros::delay(20);
  }
  this->state = State::AT_SPEED;
}
} // namespace robot
