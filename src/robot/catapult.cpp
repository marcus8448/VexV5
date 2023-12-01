#include "catapult.hpp"

#include "debug/logger.hpp"

namespace robot {
Catapult::Catapult(int8_t motorPort, int8_t rotationPort) : motor(device::DirectMotor(motorPort, "Catapult")), rotation(device::Rotation(rotationPort, "CatapultR")), pid(1.0, 5.0, 1.0,0.0, 1.0) {}

void Catapult::launchOne(const uint16_t speed) {
  this->state = SINGLE_LAUNCH;
  this->speed = static_cast<int16_t>(speed);
  this->pid.resetState();
}

void Catapult::launchRepeat(const uint16_t speed) {
  this->state = REPEAT_LAUNCH;
  this->speed = static_cast<int16_t>(speed);
  this->pid.resetState();
}

void Catapult::hold() {
  this->state = HOLD;
  this->pid.resetState();
}

int16_t Catapult::getSpeed() const { return this->speed; }

void Catapult::updateTargeting(control::input::Controller *controller) {}

void Catapult::updateState() {
  switch (this->state) {
  case HOLD: {
    const double update = this->pid.update(15000, this->rotation.getRotation() / 100.0);
    if (update < 0.0) {
      this->motor.moveMillivolts(this->speed);
    }
    this->motor.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
    this->motor.moveMillivolts(update);
  }
    break;
  case REPEAT_LAUNCH:
    this->motor.moveMillivolts(this->speed);
    break;
  case SINGLE_LAUNCH:
    this->motor.moveMillivolts(this->speed);
    break;
  }
  logger::info("CATA: {:.2f} @ {:.2f}", this->motor.getEfficiency(), this->rotation.getRotation());
  this->motor.moveMillivolts(this->speed);
}
} // namespace robot
