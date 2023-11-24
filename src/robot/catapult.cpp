#include "robot/catapult.hpp"

#include "debug/logger.hpp"

namespace robot {
Catapult::Catapult(int8_t motorPort) : motor(device::DirectMotor(motorPort, "Catapult")) {}

void Catapult::stop() {
  this->state = RELEASE;
  this->speed = 0;
}

void Catapult::launchOne(const uint16_t speed) {
  this->state = SINGLE_LAUNCH;
  this->speed = static_cast<int16_t>(speed);
}

void Catapult::launchRepeat(const uint16_t speed) {
  this->state = REPEAT_LAUNCH;
  this->speed = static_cast<int16_t>(speed);
}

void Catapult::release() {
  this->state = RELEASE;
  this->speed = static_cast<int16_t>(-speed);
}

void Catapult::hold() {
  this->state = HOLD;
  this->speed = static_cast<int16_t>(-speed);
}

int16_t Catapult::getSpeed() const { return this->speed; }

void Catapult::updateTargeting(control::input::Controller *controller) {}

void Catapult::updateState() {
  switch (this->state) {
  case RELEASE:
    this->motor.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    this->motor.moveMillivolts(0);
    this->motor.tare();
    break;
  case HOLD:
    this->motor.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
    this->motor.moveAbsolute(150.0, 100.0);
    break;
  case REPEAT_LAUNCH:
    this->motor.moveMillivolts(this->speed);
    break;
  case SINGLE_LAUNCH:
    this->motor.moveMillivolts(this->speed);
    break;
  }
  logger::info("eff: {:.2f}", this->motor.getEfficiency());
  this->motor.moveMillivolts(this->speed);
}
} // namespace robot
