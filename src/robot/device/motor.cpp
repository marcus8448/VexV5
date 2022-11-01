#include "robot/device/motor.hpp"

#include "logger.hpp"
#include <utility>

namespace robot::device {
Motor::Motor(pros::Motor motor): motor(std::move(motor)), gearset(motor.get_gearing()), brakeMode(motor.get_brake_mode()), reversed(motor.is_reversed()), port(motor.get_port()) {
  if (this->motor.get_encoder_units() != pros::E_MOTOR_ENCODER_DEGREES) {
    logger::error("Converting motor to degrees encoder units!");
    this->motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES)
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
} // namespace robot::device