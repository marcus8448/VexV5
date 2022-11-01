#include "robot/device/motor.hpp"

#include "logger.hpp"
#include "pros/rtos.hpp"
#include <cmath>
#include <utility>

#define TEST_FREQUENCY 50

namespace robot::device {
Motor::Motor(const pros::Motor motor): motor(std::move(motor)), gearset(this->motor.get_gearing()), brakeMode(this->motor.get_brake_mode()), reversed(this->motor.is_reversed()), port(this->motor.get_port()) {
  if (this->motor.get_encoder_units() != pros::E_MOTOR_ENCODER_DEGREES) {
    logger::error("Converting motor to degrees encoder units!");
    this->motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
  }
}

Motor::Motor(const uint8_t port, const pros::motor_gearset_e_t gearset, const pros::motor_brake_mode_e_t brake_mode,
             bool reversed): motor(pros::Motor(static_cast<int8_t>(port), gearset, reversed, pros::E_MOTOR_ENCODER_DEGREES)), gearset(gearset), brakeMode(brake_mode), reversed(reversed), port(port) {
  this->motor.set_brake_mode(brake_mode);
}

Motor::Motor(const uint8_t port, bool reversed): motor(pros::Motor(static_cast<int8_t>(port), pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_BRAKE, pros::E_MOTOR_ENCODER_DEGREES)), gearset(pros::E_MOTOR_GEARSET_18), brakeMode(pros::E_MOTOR_BRAKE_BRAKE), reversed(reversed), port(port) {
  this->motor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}

Motor::~Motor() {
}

void Motor::move_velocity(const int32_t target_velocity) {
  if (this->targetType != TargetType::VELOCITY && this->target != target_velocity) {
    this->target = target_velocity;
    this->targetType = TargetType::VELOCITY;
    this->targetPosition = INFINITY;
    this->motor.move_velocity(target_velocity);
  }
}

void Motor::move_voltage(const int32_t target_voltage) {
  if (this->targetType != TargetType::VOLTAGE && this->target != target_voltage) {
    this->target = target_voltage;
    this->targetType = TargetType::VOLTAGE;
    this->targetPosition = INFINITY;
    this->motor.move_voltage(target_voltage);
  }
}

void Motor::move_absolute(double target_position, int32_t target_velocity) {
  if (this->targetPosition != target_position) {
    this->targetType = TargetType::VELOCITY;
    this->target = target_velocity;
    this->targetPosition = target_position;
    this->motor.move_absolute(target_position, target_velocity);
  }
}

void Motor::move_realative(double target_position, int32_t target_velocity) {
  if (this->targetPosition != target_position) {
    this->targetType = TargetType::VELOCITY;
    this->target = target_velocity;
    this->targetPosition = this->get_position() + target_position;
    this->motor.move_absolute(this->targetPosition, target_velocity);
  }
}

void Motor::move_realative_target(double target_position, int32_t target_velocity) {
  if (this->targetPosition != target_position) {
    this->targetType = TargetType::VELOCITY;
    this->target = target_velocity;
    this->targetPosition += target_position;
    this->motor.move_absolute(this->targetPosition, target_velocity);
  }
}

bool Motor::is_at_velocity(int32_t target_velocity) const {
  if (this->targetType == TargetType::VELOCITY && this->target < target_velocity) {
    logger::error("Motor target velocity is less than requested target velocity!");
  }
  return this->get_velocity() >= target_velocity;
}

void Motor::await_velocity(int32_t target_velocity, int16_t timeout_millis) const {
  for (uint16_t i = 0; i < timeout_millis / TEST_FREQUENCY; i++) {
    if (this->is_at_velocity(target_velocity)) break;
    pros::delay(TEST_FREQUENCY);
  }
}

[[nodiscard]] double Motor::get_velocity() const {
  return this->motor.get_actual_velocity();
}

[[nodiscard]] double Motor::get_efficiency() const {
  return this->motor.get_efficiency();
}

[[nodiscard]] int32_t Motor::get_target_velocity() const {
  if (this->targetType != TargetType::VELOCITY) {
    logger::error("Requested target velocity while motor is targetting voltage!");
  }
  return this->target;
}

[[nodiscard]] int32_t Motor::get_target_voltage() const {
  if (this->targetType != TargetType::VOLTAGE) {
    logger::error("Requested target voltage while motor is targetting velocity!");
  }
  return this->target;
}

[[nodiscard]] double Motor::get_position() const {
  return this->motor.get_position();
}

[[nodiscard]] double Motor::get_target_position() const {
  return this->targetPosition;
}

[[nodiscard]] pros::motor_brake_mode_e_t Motor::get_brake_mode() const {
  return this->brakeMode;
}

[[nodiscard]] pros::motor_gearset_e_t Motor::get_gearset() const {
  return this->gearset;
}

[[nodiscard]] bool Motor::is_reversed() const {
  return this->reversed;
}

[[nodiscard]] uint8_t Motor::get_port() const {
  return this->port;
}

void Motor::await_target(int16_t timeout_millis) const {
  for (uint16_t i = 0; i < timeout_millis / TEST_FREQUENCY; i++) {
    if (this->is_at_target()) break;
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

void Motor::tare() {
  this->motor.tare_position();
}

void Motor::stop() {
  this->move_voltage(0);
}

[[nodiscard]] TargetType Motor::get_target_type() const {
  return this->targetType;
}

const pros::Motor Motor::get_raw_motor() const {
  return this->motor;
}
} // namespace robot::device
