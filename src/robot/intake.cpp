#include "intake.hpp"

namespace robot {
Intake::Intake(int8_t motorPort) : motor(device::DirectMotor(motorPort, "Intake")) {}

void Intake::stop() { this->speed = 0; }

void Intake::intake(uint16_t speed) { this->speed = static_cast<int16_t>(speed); }

void Intake::release(uint16_t speed) { this->speed = static_cast<int16_t>(-static_cast<int16_t>(speed)); }

int16_t Intake::getSpeed() const { return this->speed; }

void Intake::updateTargeting(control::input::Controller *controller) {
  if (controller->r1Pressed()) {
    this->intake();
  } else if (controller->r2Pressed()) {
    this->release();
  } else {
    this->stop();
  }
}

void Intake::updateState() { this->motor.moveMillivolts(this->speed); }
} // namespace robot
