#include "robot/device/motor.hpp"
#include "include/debug/logger.hpp"
#include "pros/motors.h"
#include "robot/pid/vex_pid.hpp"
#include <cerrno>
#include <cmath>

#define TEST_FREQUENCY 50

#define DEFAULT_MOTOR_GEARSET pros::E_MOTOR_GEARSET_18
#define DEFAULT_MOTOR_BRAKE pros::E_MOTOR_BRAKE_BRAKE
#define MOTOR_ENCODER_UNITS pros::E_MOTOR_ENCODER_DEGREES

namespace robot::device {
Motor::Motor(const uint8_t port, const char *name, const pros::motor_gearset_e_t gearset,
             const pros::motor_brake_mode_e_t brake_mode, bool reversed)
    : Device(port, name), gearset(gearset), maxVelocity(get_gearset_max_velocity(gearset)), brakeMode(brake_mode),
      reversed(reversed) {
  pros::c::motor_set_gearing(this->port, this->gearset);
  pros::c::motor_set_reversed(this->port, this->reversed); // todo: manually reverse
  pros::c::motor_set_encoder_units(this->port, MOTOR_ENCODER_UNITS);
  pros::c::motor_set_brake_mode(this->port, this->brakeMode);
  this->controller = new robot::VexPid(this->port);
}

Motor::Motor(const uint8_t port, const char *name, bool reversed)
    : Device(port, name), gearset(DEFAULT_MOTOR_GEARSET), maxVelocity(get_gearset_max_velocity(DEFAULT_MOTOR_GEARSET)),
      brakeMode(DEFAULT_MOTOR_BRAKE), reversed(reversed) {
  pros::c::motor_set_gearing(this->port, DEFAULT_MOTOR_GEARSET);
  pros::c::motor_set_reversed(this->port, this->reversed); // todo: manually reverse
  pros::c::motor_set_encoder_units(this->port, MOTOR_ENCODER_UNITS);
  pros::c::motor_set_brake_mode(this->port, DEFAULT_MOTOR_BRAKE);
}

Motor::~Motor() = default;

void Motor::update() {
  if (this->controller != nullptr) {
    this->controller->update();
  }
}

void Motor::move_velocity(int16_t velocity) {
  if (velocity > this->maxVelocity) {
    warn("Target velocity %i is over max velocity %i!", velocity, this->maxVelocity);
    velocity = this->maxVelocity;
  } else if (velocity < -this->maxVelocity) {
    warn("Target velocity %i is over max velocity -%i!", velocity, this->maxVelocity);
    velocity = static_cast<int16_t>(-this->maxVelocity);
  }
  if (this->targetType != TargetType::VELOCITY || this->target != velocity) {
    this->target = velocity;
    this->targetType = TargetType::VELOCITY;
    this->targetPosition = INFINITY;
    if (this->controller != nullptr) {
      this->controller->target_velocity(velocity);
    }
  }
}

void Motor::move_millivolts(int16_t mV) {
  if (mV > MOTOR_MAX_MILLIVOLTS) {
    warn("Target voltage %imV is over max voltage 12000mV!", mV);
    mV = MOTOR_MAX_MILLIVOLTS;
  } else if (mV < -MOTOR_MAX_MILLIVOLTS) {
    warn("Target voltage %imV is over max voltage -12000mV!", mV);
    mV = -MOTOR_MAX_MILLIVOLTS;
  }
  if (this->targetType != TargetType::VOLTAGE || this->target != mV) {
    debug("Targeting %imV", mV);
    this->target = mV;
    this->targetType = TargetType::VOLTAGE;
    this->targetPosition = INFINITY;
    pros::c::motor_move_voltage(this->port, mV);
  }
}

void Motor::move_percentage(double percent) {
  if (percent > 100.0) {
    percent = 100.0;
  } else if (percent < -100.0) {
    percent = -100.0;
  }
  this->move_millivolts(static_cast<int16_t>((percent * MOTOR_MAX_MILLIVOLTS) / 100.0));
}

void Motor::move_absolute(double position, int16_t velocity) {
  velocity = static_cast<int16_t>(std::abs(velocity));
  if (velocity > this->maxVelocity) {
    warn("Target velocity %i is over max velocity %i!", velocity, this->maxVelocity);
    velocity = this->maxVelocity;
  } else if (velocity == 0) {
    error("Target velocity is zero!");
  }
  if (this->targetType != TargetType::VELOCITY || this->target != velocity || this->targetPosition != position) {
    this->targetType = TargetType::VELOCITY;
    this->target = velocity;
    this->targetPosition = position;
    pros::c::motor_move_absolute(this->port, position, velocity);
  }
}

void Motor::move_relative(double amount, int16_t velocity) {
  this->move_absolute(this->get_position() + amount, velocity);
}

void Motor::move_target_relative(double amount, int16_t velocity) {
  this->move_absolute(this->get_target_position() + amount, velocity);
}

void Motor::set_reversed(const bool reverse) {
  if (this->reversed != reverse) {
    this->reversed = reverse;
    pros::c::motor_set_reversed(this->port, reverse);
  }
}

void Motor::set_brake_mode(pros::motor_brake_mode_e brake_mode) {
  if (this->brakeMode != brake_mode) {
    this->brakeMode = brake_mode;
    pros::c::motor_set_brake_mode(this->port, this->brakeMode);
  }
}

[[nodiscard]] double Motor::get_velocity() const { return pros::c::motor_get_actual_velocity(this->port); }

[[nodiscard]] double Motor::get_efficiency() const { return pros::c::motor_get_efficiency(this->port); }

[[nodiscard]] int32_t Motor::get_target_velocity() const {
  if (this->targetType != TargetType::VELOCITY) {
    error("Requested target velocity while motor is targeting voltage!");
  }
  return this->target;
}

[[nodiscard]] int32_t Motor::get_target_voltage() const {
  if (this->targetType != TargetType::VOLTAGE) {
    error("Requested target voltage while motor is targeting velocity!");
  }
  return this->target;
}

[[nodiscard]] double Motor::get_position() const { return pros::c::motor_get_position(this->port); }

[[nodiscard]] double Motor::get_target_position() const { return this->targetPosition; }

[[nodiscard]] pros::motor_brake_mode_e_t Motor::get_brake_mode() const { return this->brakeMode; }

[[nodiscard]] pros::motor_gearset_e_t Motor::get_gearset() const { return this->gearset; }

[[nodiscard]] bool Motor::is_reversed() const { return this->reversed; }

[[nodiscard]] bool Motor::is_connected() const {
  errno = 0;
  return pros::c::motor_get_voltage(this->port) != INT32_MAX && checkConnect();
}

[[nodiscard]] double Motor::get_temperature() const { return pros::c::motor_get_temperature(this->port); }

[[nodiscard]] double Motor::get_power() const { return pros::c::motor_get_power(this->port); }

[[nodiscard]] double Motor::get_torque() const { return pros::c::motor_get_torque(this->port); }

[[nodiscard]] int32_t Motor::get_current_draw() const { return pros::c::motor_get_current_draw(this->port); }

[[nodiscard]] int32_t Motor::get_voltage() const { return pros::c::motor_get_voltage(this->port); }

[[nodiscard]] int32_t Motor::get_current_limit() const { return pros::c::motor_get_current_limit(this->port); }

[[nodiscard]] int32_t Motor::get_voltage_limit() const { return pros::c::motor_get_voltage_limit(this->port); }

void Motor::await_target(int16_t timeout_millis) const {
  for (uint16_t i = 0; i < timeout_millis / TEST_FREQUENCY; i++) {
    if (this->is_at_target())
      break;
    pros::delay(TEST_FREQUENCY);
  }
}

[[nodiscard]] bool Motor::is_at_target() const {
  if (this->get_target_position() != INFINITY) {
    return std::abs(this->get_position() - this->targetPosition) < 1.0 ||
           (std::abs(this->get_position() - this->targetPosition) < 5.0 && this->get_efficiency() == 0.0);
  } else {
    return std::abs(this->get_velocity() - this->get_target_velocity()) < 10.0;
  }
}

void Motor::reconfigure() const {
  pros::c::motor_set_brake_mode(this->port, this->brakeMode);
  pros::c::motor_set_encoder_units(this->port, MOTOR_ENCODER_UNITS);
  pros::c::motor_set_gearing(this->port, this->gearset);
  pros::c::motor_set_reversed(this->port, this->reversed);
}

void Motor::tare() { pros::c::motor_set_zero_position(this->port, pros::c::motor_get_position(this->port)); }

void Motor::brake() {
  if (this->targetPosition != INFINITY || this->targetType != VOLTAGE || this->target != 0) {
    this->target = 0;
    this->targetType = VOLTAGE;
    this->targetPosition = INFINITY;
    if (this->controller != nullptr) {
      this->controller->target_velocity(0.0);
    }
    pros::c::motor_brake(this->port);
  }
}

[[nodiscard]] Motor::TargetType Motor::get_target_type() const { return this->targetType; }

int16_t get_gearset_max_velocity(const pros::motor_gearset_e_t gearset) {
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
