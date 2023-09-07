#include "robot/intake.hpp"

namespace robot {
Intake::Intake(int8_t leftPort, int8_t rightPort)
    : leftMotor(device::Motor(leftPort, "Intake L")), rightMotor(device::Motor(rightPort, "Intake R", true)),
      pneumatic(device::PneumaticPiston('A', "Launcher")) {}

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

void Intake::updateState() {
  this->leftMotor.moveMillivolts(this->speed);
  this->rightMotor.moveMillivolts(this->speed);
  if (this->speed < 0) {
    this->pneumatic.extend();
  } else {
    this->pneumatic.contract();
  }
}
} // namespace robot