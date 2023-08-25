#include "robot/device/motor.hpp"
#include "debug/logger.hpp"
#include "pros/apix.h"
#include "pros/motors.h"
#include <cerrno>
#include <cmath>

namespace robot::device {
Motor::Motor(uint8_t port, const char *name, bool reversed, pros::motor_gearset_e_t gearset,
             pros::motor_brake_mode_e_t brake_mode)
    : Device("Motor", name, reversed ? -port : port), gearset(gearset), maxVelocity(gearsetMaxVelocity(gearset)), brakeMode(brake_mode) {
  pros::c::motor_set_gearing(this->port, this->gearset);
  pros::c::motor_set_encoder_units(this->port, MOTOR_ENCODER_UNITS);
  pros::c::motor_set_brake_mode(this->port, this->brakeMode);
}

void Motor::moveVelocity(int16_t velocity) {
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
    pros::c::motor_move_velocity(this->port, velocity);
  }
}

void Motor::moveMillivolts(int16_t mV) {
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

void Motor::moveAbsolute(double position, int16_t velocity) {
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

void Motor::moveRelative(double amount, int16_t velocity) {
  this->moveAbsolute(this->getPosition() + amount, velocity);
}

void Motor::moveTargetRelative(double amount, int16_t velocity) {
  this->moveAbsolute(this->getTargetPosition() + amount, velocity);
}

void Motor::setBrakeMode(pros::motor_brake_mode_e brake_mode) {
  if (this->brakeMode != brake_mode) {
    this->brakeMode = brake_mode;
    pros::c::motor_set_brake_mode(this->port, this->brakeMode);
  }
}

[[nodiscard]] double Motor::getVelocity() const { return pros::c::motor_get_actual_velocity(this->port); }

[[nodiscard]] double Motor::getEfficiency() const { return pros::c::motor_get_efficiency(this->port); }

[[nodiscard]] int32_t Motor::getTargetVelocity() const {
  if (this->targetType != TargetType::VELOCITY) {
    error("Requested target velocity while motor is targeting voltage!");
  }
  return this->target;
}

[[nodiscard]] int32_t Motor::getTargetVoltage() const {
  if (this->targetType != TargetType::VOLTAGE) {
    error("Requested target voltage while motor is targeting velocity!");
  }
  return this->target;
}

[[nodiscard]] double Motor::getPosition() const { return pros::c::motor_get_position(this->port); }

[[nodiscard]] double Motor::getTargetPosition() const { return this->targetPosition; }

[[nodiscard]] pros::motor_brake_mode_e_t Motor::getBrakeMode() const { return this->brakeMode; }

[[nodiscard]] pros::motor_gearset_e_t Motor::getGearset() const { return this->gearset; }

[[nodiscard]] bool Motor::isConnected() const {
  errno = 0;
  return pros::c::motor_get_position(this->port) != PROS_ERR_F && checkConnect();
}

[[nodiscard]] double Motor::getTemperature() const { return pros::c::motor_get_temperature(this->port); }

[[nodiscard]] double Motor::getPower() const { return pros::c::motor_get_power(this->port); }

[[nodiscard]] double Motor::getTorque() const { return pros::c::motor_get_torque(this->port); }

[[nodiscard]] int32_t Motor::getCurrentDraw() const { return pros::c::motor_get_current_draw(this->port); }

[[nodiscard]] int32_t Motor::getVoltage() const { return pros::c::motor_get_voltage(this->port); }

[[nodiscard]] int32_t Motor::getCurrentLimit() const { return pros::c::motor_get_current_limit(this->port); }

[[nodiscard]] int32_t Motor::getVoltageLimit() const { return pros::c::motor_get_voltage_limit(this->port); }

void Motor::reconfigure() const {
  pros::c::motor_set_brake_mode(this->port, this->brakeMode);
  pros::c::motor_set_encoder_units(this->port, MOTOR_ENCODER_UNITS);
  pros::c::motor_set_gearing(this->port, this->gearset);
}

void Motor::tare() { pros::c::motor_tare_position(this->port); }

void Motor::brake() {
  if (this->targetPosition != INFINITY || this->targetType != VOLTAGE || this->target != 0) {
    this->target = 0;
    this->targetType = VOLTAGE;
    this->targetPosition = INFINITY;
    pros::c::motor_brake(this->port);
  }
}

[[nodiscard]] Motor::TargetType Motor::getTargetType() const { return this->targetType; }

constexpr int16_t Motor::gearsetMaxVelocity(pros::motor_gearset_e_t gearset) {
  switch (gearset) {
  case pros::E_MOTOR_GEARSET_36:
    return 100;
  case pros::E_MOTOR_GEARSET_18:
    return 200;
  case pros::E_MOTOR_GEARSET_06:
    return 600;
  default:
    return 0;
  }
}

double clampMv(double value, double moveMin) {
  if (std::abs(value) < moveMin) value = value < 0 ? -600 : 600;
  return value > MOTOR_MAX_MILLIVOLTS    ? MOTOR_MAX_MILLIVOLTS
         : value < -MOTOR_MAX_MILLIVOLTS ? -MOTOR_MAX_MILLIVOLTS
                                         : value;
}

PID::PID(double Kp, double Ki, double Kd, double integralRange, double acceptableError)
    : kp(Kp), ki(Ki), kd(Kd), integralRange(integralRange), acceptableError(acceptableError) {}

void PID::resetState() {
  this->integral = 0;
  this->error = 0;
}

double PID::update(double target, double value) {
  this->error = target - value;
  if (std::abs(this->error) < this->acceptableError || value == PROS_ERR_F)
    return 0.0;

  this->prevError = this->error;
  this->integral += this->error;
  if (std::signbit(this->error) != std::signbit(this->prevError) || std::abs(this->error) > this->integralRange) {
    this->integral = 0;
  }
  info("%.2f %.2f/%.2f, %.1f %.1f %.1f * %.2f %.2f %.2f -> %.2f/%.2f/%.2f -> %.2f", this->error, value, target, this->kp, this->ki, this->kd, this->error, this->integral, this->error - this->prevError, this->error * this->kp, this->integral * this->ki, (this->error - this->prevError) * this->kd, clampMv(this->error * this->kp + this->integral * this->ki + (this->error - this->prevError) * this->kd, this->moveMin));
  return clampMv(this->error * this->kp + this->integral * this->ki + (this->error - this->prevError) * this->kd, this->moveMin);
}

double PID::getError() const { return error; }
} // namespace robot::device
