#include "robot/device/motor.hpp"
#include "debug/logger.hpp"
#include "pros/motors.h"
#include "debug/error.hpp"

namespace robot::device {
DirectMotor::DirectMotor(int8_t port, const char *name, bool reversed, pros::motor_gearset_e_t gearset,
             pros::motor_brake_mode_e_t brake_mode)
    : Device("DirectMotor", name, static_cast<int8_t>(reversed ? -port : port)), gearset(gearset),
      maxVelocity(gearsetMaxVelocity(gearset)), brakeMode(brake_mode) {
  pros::c::motor_set_gearing(this->port, this->gearset);
  pros::c::motor_set_encoder_units(this->port, pros::E_MOTOR_ENCODER_DEGREES);
  pros::c::motor_set_brake_mode(this->port, this->brakeMode);
}

void DirectMotor::moveVelocity(int16_t velocity) {
  if (velocity > this->maxVelocity) {
    logger::warn("Target velocity %i is over max velocity %i!", velocity, this->maxVelocity);
    velocity = this->maxVelocity;
  } else if (velocity < -this->maxVelocity) {
    logger::warn("Target velocity %i is over max velocity -%i!", velocity, this->maxVelocity);
    velocity = static_cast<int16_t>(-this->maxVelocity);
  }
  if (this->targetType != Motor::TargetType::VELOCITY || this->target != velocity) {
    this->target = velocity;
    this->targetType = Motor::TargetType::VELOCITY;
    this->targetPosition = std::numeric_limits<double>::infinity();
    pros::c::motor_move_velocity(this->port, velocity);
  }
}

void DirectMotor::moveMillivolts(int16_t mV) {
  if (mV > MAX_MILLIVOLTS) {
    logger::warn("Target voltage %imV is over max voltage %imV!", mV, MAX_MILLIVOLTS);
    mV = MAX_MILLIVOLTS;
  } else if (mV < -MAX_MILLIVOLTS) {
    logger::warn("Target voltage %imV is over max voltage -%imV!", mV, MAX_MILLIVOLTS);
    mV = -MAX_MILLIVOLTS;
  }
  if (this->targetType != Motor::TargetType::VOLTAGE || this->target != mV) {
    logger::debug("Targeting %imV", mV);
    this->target = mV;
    this->targetType = Motor::TargetType::VOLTAGE;
    this->targetPosition = std::numeric_limits<double>::infinity();
    pros::c::motor_move_voltage(this->port, mV);
  }
}

void DirectMotor::moveAbsolute(double position, int16_t velocity) {
  velocity = static_cast<int16_t>(std::abs(velocity));
  if (velocity > this->maxVelocity) {
    logger::warn("Target velocity %i is over max velocity %i!", velocity, this->maxVelocity);
    velocity = this->maxVelocity;
  } else if (velocity == 0) {
    logger::error("Target velocity is zero!");
  }
  if (this->targetType != Motor::TargetType::VELOCITY || this->target != velocity || this->targetPosition != position) {
    this->targetType = Motor::TargetType::VELOCITY;
    this->target = velocity;
    this->targetPosition = position;
    pros::c::motor_move_absolute(this->port, position, velocity);
  }
}

void DirectMotor::moveRelative(double amount, int16_t velocity) {
  this->moveAbsolute(this->getPosition() + amount, velocity);
}

void DirectMotor::moveTargetRelative(double amount, int16_t velocity) {
  this->moveAbsolute(this->getTargetPosition() + amount, velocity);
}

void DirectMotor::setBrakeMode(pros::motor_brake_mode_e brake_mode) {
  if (this->brakeMode != brake_mode) {
    this->brakeMode = brake_mode;
    pros::c::motor_set_brake_mode(this->port, this->brakeMode);
  }
}

[[nodiscard]] double DirectMotor::getVelocity() const { return pros::c::motor_get_actual_velocity(this->port); }

[[nodiscard]] double DirectMotor::getEfficiency() const { return pros::c::motor_get_efficiency(this->port); }

[[nodiscard]] int32_t DirectMotor::getTargetVelocity() const {
  if (this->targetType != Motor::TargetType::VELOCITY) {
    logger::error("Requested target velocity while motor is targeting voltage!");
  }
  return this->target;
}

[[nodiscard]] int32_t DirectMotor::getTargetVoltage() const {
  if (this->targetType != Motor::TargetType::VOLTAGE) {
    logger::error("Requested target voltage while motor is targeting velocity!");
  }
  return this->target;
}

[[nodiscard]] double DirectMotor::getPosition() const { return pros::c::motor_get_position(this->port); }

[[nodiscard]] double DirectMotor::getTargetPosition() const { return this->targetPosition; }

[[nodiscard]] pros::motor_brake_mode_e_t DirectMotor::getBrakeMode() const { return this->brakeMode; }

[[nodiscard]] pros::motor_gearset_e_t DirectMotor::getGearset() const { return this->gearset; }

[[nodiscard]] bool DirectMotor::isConnected() const {
  if (error::check(pros::c::motor_get_position(this->port))) {
    return !error::isDisconnected();
  }
  return true;
}

[[nodiscard]] double DirectMotor::getTemperature() const { return pros::c::motor_get_temperature(this->port); }

[[nodiscard]] double DirectMotor::getPower() const { return pros::c::motor_get_power(this->port); }

[[nodiscard]] double DirectMotor::getTorque() const { return pros::c::motor_get_torque(this->port); }

[[nodiscard]] int32_t DirectMotor::getCurrentDraw() const { return pros::c::motor_get_current_draw(this->port); }

[[nodiscard]] int32_t DirectMotor::getVoltage() const { return pros::c::motor_get_voltage(this->port); }

void DirectMotor::reconfigure() const {
  pros::c::motor_set_brake_mode(this->port, this->brakeMode);
  pros::c::motor_set_encoder_units(this->port, pros::E_MOTOR_ENCODER_DEGREES);
  pros::c::motor_set_gearing(this->port, this->gearset);
}

void DirectMotor::tare() { pros::c::motor_tare_position(this->port); }

void DirectMotor::brake() {
  if (this->targetPosition != std::numeric_limits<double>::infinity() || this->targetType != Motor::TargetType::VOLTAGE || this->target != 0) {
    this->target = 0;
    this->targetType = Motor::TargetType::VOLTAGE;
    this->targetPosition = std::numeric_limits<double>::infinity();
    pros::c::motor_brake(this->port);
  }
}

template <uint8_t MOTORS> MotorGroup<MOTORS>::MotorGroup(std::array<int8_t, MOTORS> motors, const char *name, pros::motor_gearset_e_t gearset, pros::motor_brake_mode_e_t brake_mode)
    : maxVelocity(Motor::gearsetMaxVelocity(gearset)), brakeMode(brake_mode), motors(motors) {
  int i = 0;
  for (const auto &port : this->motors) {
    auto *motor = new DirectMotor(port, fmt::static_string_format("%s%i", name, ++i), false, gearset);
    motor->setBrakeMode(brake_mode);
    // leak
  }
}

template <uint8_t MOTORS> void MotorGroup<MOTORS>::moveVelocity(int16_t velocity) {
  if (velocity > this->maxVelocity) {
    logger::warn("Target velocity %i is over max velocity %i!", velocity, this->maxVelocity);
    velocity = this->maxVelocity;
  } else if (velocity < -this->maxVelocity) {
    logger::warn("Target velocity %i is over max velocity -%i!", velocity, this->maxVelocity);
    velocity = static_cast<int16_t>(-this->maxVelocity);
  }
  if (this->targetType != Motor::TargetType::VELOCITY || this->target != velocity) {
    this->target = velocity;
    this->targetType = Motor::TargetType::VELOCITY;
    this->targetPosition = std::numeric_limits<double>::infinity();
    for (const auto &port : motors) {
      pros::c::motor_move_velocity(port, velocity);
    }
  }
}

template <uint8_t MOTORS> void MotorGroup<MOTORS>::moveMillivolts(int16_t mV) {
  if (mV > MAX_MILLIVOLTS) {
    logger::warn("Target voltage %imV is over max voltage %imV!", mV, MAX_MILLIVOLTS);
    mV = MAX_MILLIVOLTS;
  } else if (mV < -MAX_MILLIVOLTS) {
    logger::warn("Target voltage %imV is over max voltage -%imV!", mV, MAX_MILLIVOLTS);
    mV = -MAX_MILLIVOLTS;
  }
  if (this->targetType != Motor::TargetType::VOLTAGE || this->target != mV) {
    logger::debug("Targeting %imV", mV);
    this->target = mV;
    this->targetType = Motor::TargetType::VOLTAGE;
    this->targetPosition = std::numeric_limits<double>::infinity();
    for (const auto &port : this->motors) {
      pros::c::motor_move_voltage(port, mV);
    }
  }
}

template <uint8_t MOTORS> void MotorGroup<MOTORS>::moveAbsolute(double position, int16_t velocity) {
  velocity = static_cast<int16_t>(std::abs(velocity));
  if (velocity > this->maxVelocity) {
    logger::warn("Target velocity %i is over max velocity %i!", velocity, this->maxVelocity);
    velocity = this->maxVelocity;
  } else if (velocity == 0) {
    logger::error("Target velocity is zero!");
  }
  if (this->targetType != Motor::TargetType::VELOCITY || this->target != velocity || this->targetPosition != position) {
    this->targetType = Motor::TargetType::VELOCITY;
    this->target = velocity;
    this->targetPosition = position;
    for (const auto &port : this->motors) {
      pros::c::motor_move_absolute(port, position, velocity);
    }
  }
}

template <uint8_t MOTORS> void MotorGroup<MOTORS>::moveRelative(double amount, int16_t velocity) {
  this->moveAbsolute(this->getPosition() + amount, velocity);
}

template <uint8_t MOTORS> void MotorGroup<MOTORS>::moveTargetRelative(double amount, int16_t velocity) {
  this->moveAbsolute(this->getTargetPosition() + amount, velocity);
}

template <uint8_t MOTORS> void MotorGroup<MOTORS>::setBrakeMode(pros::motor_brake_mode_e brake_mode) {
  for (const auto &port : this->motors) {
    pros::c::motor_set_brake_mode(port, brake_mode);
  }
}

template <uint8_t MOTORS> [[nodiscard]] double MotorGroup<MOTORS>::getVelocity() const {
  double velocity = 0.0;
  uint8_t count = 0;
  for (const auto &port : this->motors) {
    double vel = pros::c::motor_get_actual_velocity(port);
    if (vel != std::numeric_limits<double>::infinity()) {
      velocity += vel;
      count++;
    }
  }
  if (count == 0) return 0.0;
  return velocity / count;
}

template <uint8_t MOTORS> [[nodiscard]] double MotorGroup<MOTORS>::getEfficiency() const {
  double efficiency = 0.0;
  uint8_t count = 0;
  for (const auto &port : this->motors) {
    double eff = pros::c::motor_get_efficiency(port);
    if (eff != std::numeric_limits<double>::infinity()) {
      efficiency += eff;
      count++;
    }
  }
  if (count == 0) return 0.0;
  return efficiency / count;
}

template <uint8_t MOTORS> [[nodiscard]] int32_t MotorGroup<MOTORS>::getTargetVelocity() const {
  for (const auto &port : this->motors) {
    int32_t vel = pros::c::motor_get_target_velocity(port);
    if (vel != std::numeric_limits<int32_t>::max()) {
      return vel;
    }
  }
  return 0.0;
}

template <uint8_t MOTORS> [[nodiscard]] int32_t MotorGroup<MOTORS>::getTargetVoltage() const {
  if (this->targetType != Motor::TargetType::VOLTAGE) {
    logger::error("Requested target voltage while motor is targeting velocity!");
  }
  return this->target;
}

template <uint8_t MOTORS> [[nodiscard]] double MotorGroup<MOTORS>::getPosition() const {
  double position = 0.0;
  uint8_t count = 0;
  for (const auto &port : this->motors) {
    double pos = pros::c::motor_get_position(port);
    if (pos != std::numeric_limits<double>::infinity()) {
      position += pos;
      count++;
    }
  }
  if (count == 0) return 0.0;
  return position / count;
}

template <uint8_t MOTORS> [[nodiscard]] double MotorGroup<MOTORS>::getTargetPosition() const {
  return this->targetPosition;
}

template <uint8_t MOTORS> [[nodiscard]] pros::motor_brake_mode_e_t MotorGroup<MOTORS>::getBrakeMode() const { return pros::c::motor_get_brake_mode(this->motors[0]); }

template <uint8_t MOTORS> [[nodiscard]] double MotorGroup<MOTORS>::getTemperature() const {
  double temperature = 0.0;
  uint8_t count = 0;
  for (const auto &port : this->motors) {
    double tmp = pros::c::motor_get_temperature(port);
    if (tmp != std::numeric_limits<double>::infinity()) {
      temperature += tmp;
      count++;
    }
  }
  if (count == 0) return 0.0;
  return temperature / count;
}

template <uint8_t MOTORS> [[nodiscard]] double MotorGroup<MOTORS>::getPower() const {
  double power = 0.0;
  uint8_t count = 0;
  for (const auto &port : this->motors) {
    double pwr = pros::c::motor_get_power(port);
    if (pwr != std::numeric_limits<double>::infinity()) {
      power += pwr;
      count++;
    }
  }
  if (count == 0) return 0.0;
  return power / count;
}

template <uint8_t MOTORS> [[nodiscard]] double MotorGroup<MOTORS>::getTorque() const {
  double torque = 0.0;
  uint8_t count = 0;
  for (const auto &port : this->motors) {
    double tqe = pros::c::motor_get_torque(port);
    if (tqe != std::numeric_limits<double>::infinity()) {
      torque += tqe;
      count++;
    }
  }
  if (count == 0) return 0.0;
  return torque / count;
}

template <uint8_t MOTORS> [[nodiscard]] int32_t MotorGroup<MOTORS>::getCurrentDraw() const {
  int32_t current = 0;
  uint8_t count = 0;
  for (const auto &port : this->motors) {
    int32_t cur = pros::c::motor_get_current_draw(port);
    if (cur != std::numeric_limits<int32_t>::max()) {
      current += cur;
      count++;
    }
  }
  if (count == 0) return 0.0;
  return current / count;
}

template <uint8_t MOTORS> [[nodiscard]] int32_t MotorGroup<MOTORS>::getVoltage() const {
  int32_t voltage = 0;
  uint8_t count = 0;
  for (const auto &port : this->motors) {
    int32_t vlt = pros::c::motor_get_voltage(port);
    if (vlt != std::numeric_limits<int32_t>::max()) {
      voltage += vlt;
      count++;
    }
  }
  if (count == 0) return 0.0;
  return voltage / count;
}

template <uint8_t MOTORS> void MotorGroup<MOTORS>::tare() {
  for (auto &port : this->motors) {
    pros::c::motor_tare_position(port);
  }
}

template <uint8_t MOTORS> void MotorGroup<MOTORS>::brake() {
  for (auto &port : this->motors) {
    pros::c::motor_brake(port);
  }
}

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

template class MotorGroup<3>;
} // namespace robot::device
