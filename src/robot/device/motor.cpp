#include "robot/device/motor.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"
#include <cerrno>
#include <cmath>
#include <utility>

#define TEST_FREQUENCY 50

#define MAX_MILLIVOLTS 11750
#define DEFAULT_MOTOR_GEARSET pros::E_MOTOR_GEARSET_18
#define DEFAULT_MOTOR_BRAKE pros::E_MOTOR_BRAKE_BRAKE
#define MOTOR_ENCODER_UNITS pros::E_MOTOR_ENCODER_DEGREES

namespace robot::device {
Motor::Motor(pros::Motor motor)
    : motor(std::move(motor)), gearset(this->motor.get_gearing()), maxVelocity(get_gearset_max_velocity(this->gearset)),
      brakeMode(this->motor.get_brake_mode()), reversed(this->motor.is_reversed()), port(this->motor.get_port()) {
  if (this->motor.get_encoder_units() != MOTOR_ENCODER_UNITS) {
    logger::error("Converting motor to degrees encoder units!");
    this->motor.set_encoder_units(MOTOR_ENCODER_UNITS);
  }
}

Motor::Motor(const uint8_t port, const pros::motor_gearset_e_t gearset, const pros::motor_brake_mode_e_t brake_mode,
             bool reversed)
    : motor(pros::Motor(static_cast<int8_t>(port), gearset, reversed, MOTOR_ENCODER_UNITS)), gearset(gearset),
      maxVelocity(get_gearset_max_velocity(this->gearset)), brakeMode(brake_mode), reversed(reversed), port(port) {
  this->motor.set_brake_mode(brake_mode);
}

Motor::Motor(const uint8_t port, bool reversed)
    : motor(pros::Motor(static_cast<int8_t>(port), DEFAULT_MOTOR_GEARSET, DEFAULT_MOTOR_BRAKE, MOTOR_ENCODER_UNITS)),
      gearset(DEFAULT_MOTOR_GEARSET), maxVelocity(get_gearset_max_velocity(this->gearset)),
      brakeMode(DEFAULT_MOTOR_BRAKE), reversed(reversed), port(port) {
  this->motor.set_brake_mode(DEFAULT_MOTOR_BRAKE);
}

Motor::~Motor() = default;

void Motor::move_velocity(int16_t target_velocity) {
  if (target_velocity > this->maxVelocity) {
    logger::warn("Target velocity %i is over max velocity %i!", target_velocity, this->maxVelocity);
    target_velocity = this->maxVelocity;
  } else if (target_velocity < -this->maxVelocity) {
    logger::warn("Target velocity %i is over max velocity %i!", target_velocity, -this->maxVelocity);
    target_velocity = -this->maxVelocity;
  }
  if (this->targetType != TargetType::VELOCITY || this->target != target_velocity) {
    this->target = target_velocity;
    this->targetType = TargetType::VELOCITY;
    this->targetPosition = INFINITY;
    this->motor.move_velocity(target_velocity);
  }
}

void Motor::move_millivolts(int16_t target_voltage) {
  if (target_voltage > MAX_MILLIVOLTS) {
    logger::warn("Target voltage %imV is over max voltage 12000mV!", target_voltage);
    target_voltage = MAX_MILLIVOLTS;
  } else if (target_voltage < -MAX_MILLIVOLTS) {
    logger::warn("Target voltage %imV is over max voltage -12000mV!", target_voltage);
    target_voltage = -MAX_MILLIVOLTS;
  }
  if (this->targetType != TargetType::VOLTAGE || this->target != target_voltage) {
    this->target = target_voltage;
    this->targetType = TargetType::VOLTAGE;
    this->targetPosition = INFINITY;
    this->motor.move_voltage(target_voltage);
  }
}

void Motor::move_percentage(double percent) {
  if (percent > 100.0) {
    percent = 100.0;
  } else if (percent < -100.0) {
    percent = -100.0;
  }
  this->move_millivolts(static_cast<int16_t>(percent * MAX_MILLIVOLTS));
}

void Motor::move_absolute(double target_position, uint16_t target_velocity) {
  if (target_velocity > this->maxVelocity) {
    logger::warn("Target velocity %i is over max velocity %i!", target_velocity, this->maxVelocity);
    target_velocity = this->maxVelocity;
  } else if (target_velocity == 0) {
    logger::warn("Target velocity is zero!");
  }
  if (this->targetPosition != target_position) {
    this->targetType = TargetType::VELOCITY;
    this->target = target_velocity;
    this->targetPosition = target_position;
    this->motor.move_absolute(target_position, target_velocity);
  }
}

void Motor::move_relative(double target_position, uint16_t target_velocity) {
  if (target_velocity > this->maxVelocity) {
    logger::warn("Target velocity %i is over max velocity %i!", target_velocity, this->maxVelocity);
    target_velocity = this->maxVelocity;
  } else if (target_velocity == 0) {
    logger::warn("Target velocity is zero!");
  }
  if (this->targetPosition != target_position) {
    this->targetType = TargetType::VELOCITY;
    this->target = target_velocity;
    this->targetPosition = this->get_position() + target_position;
    this->motor.move_absolute(this->targetPosition, target_velocity);
  }
}

void Motor::move_relative_target(double target_position, uint16_t target_velocity) {
  if (target_velocity > this->maxVelocity) {
    logger::warn("Target velocity %i is over max velocity %i!", target_velocity, this->maxVelocity);
    target_velocity = this->maxVelocity;
  } else if (target_velocity == 0) {
    logger::warn("Target velocity is zero!");
  }
  if (this->targetPosition != target_position) {
    this->targetType = TargetType::VELOCITY;
    this->target = target_velocity;
    this->targetPosition += target_position;
    this->motor.move_absolute(this->targetPosition, target_velocity);
  }
}

void Motor::set_reversed(const bool reverse) {
  if (this->reversed != reverse) {
    this->reversed = reverse;
    this->motor.set_reversed(reverse);
  }
}

bool Motor::is_at_velocity(uint16_t target_velocity) const {
  if (this->targetType == TargetType::VELOCITY && std::abs(this->target) < target_velocity) {
    logger::error("Motor target velocity is less than requested target velocity!");
  }
  return std::abs(this->get_velocity()) >= target_velocity;
}

void Motor::await_velocity(uint16_t target_velocity, int16_t timeout_millis) const {
  for (uint16_t i = 0; i < timeout_millis / TEST_FREQUENCY; i++) {
    if (this->is_at_velocity(target_velocity))
      break;
    pros::delay(TEST_FREQUENCY);
  }
}

[[nodiscard]] double Motor::get_velocity() const { return this->motor.get_actual_velocity(); }

[[nodiscard]] double Motor::get_efficiency() const { return this->motor.get_efficiency(); }

[[nodiscard]] int32_t Motor::get_target_velocity() const {
  if (this->targetType != TargetType::VELOCITY) {
    logger::error("Requested target velocity while motor is targeting voltage!");
  }
  return this->target;
}

[[nodiscard]] int32_t Motor::get_target_voltage() const {
  if (this->targetType != TargetType::VOLTAGE) {
    logger::error("Requested target voltage while motor is targeting velocity!");
  }
  return this->target;
}

[[nodiscard]] double Motor::get_position() const { return this->motor.get_position(); }

[[nodiscard]] double Motor::get_target_position() const { return this->targetPosition; }

[[nodiscard]] pros::motor_brake_mode_e_t Motor::get_brake_mode() const { return this->brakeMode; }

[[nodiscard]] pros::motor_gearset_e_t Motor::get_gearset() const { return this->gearset; }

[[nodiscard]] bool Motor::is_reversed() const { return this->reversed; }

[[nodiscard]] uint8_t Motor::get_port() const { return this->port; }

[[nodiscard]] bool Motor::is_connected() { return this->motor.get_voltage() == INT32_MAX && errno == ENODEV; }

double Motor::get_temperature() const { return this->motor.get_temperature(); }

void Motor::await_target(int16_t timeout_millis) const {
  for (uint16_t i = 0; i < timeout_millis / TEST_FREQUENCY; i++) {
    if (this->is_at_target())
      break;
    pros::delay(TEST_FREQUENCY);
  }
}

[[nodiscard]] bool Motor::is_at_target() const {
  if (this->get_target_position() != INFINITY) {
    return std::abs(this->get_position() - this->targetPosition) < 5.0;
  } else {
    return std::abs(this->get_velocity() - this->get_target_velocity()) < 10.0;
  }
}

void Motor::tare() { this->motor.tare_position(); }

void Motor::stop() { this->move_millivolts(0); }

[[nodiscard]] Motor::TargetType Motor::get_target_type() const { return this->targetType; }

const pros::Motor &Motor::get_raw_motor() const { return this->motor; }

uint16_t get_gearset_max_velocity(const pros::motor_gearset_e_t gearset) {
  switch (gearset) {
  case pros::E_MOTOR_GEARSET_36:
    return 100;
  case pros::E_MOTOR_GEARSET_18:
    return 200;
  case pros::E_MOTOR_GEARSET_06:
    return 600;
  case pros::E_MOTOR_GEARSET_INVALID:
    return 0;
  }
  return 0;
}
} // namespace robot::device
