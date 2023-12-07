#include "catapult.hpp"

#include "debug/logger.hpp"

namespace robot {
Catapult::Catapult(int8_t motorPort, int8_t rotationPort)
    : motor(device::DirectMotor(motorPort, "Catapult", true, pros::E_MOTOR_GEAR_RED)),
      rotation(device::Rotation(rotationPort, "CatapultR")), pid(2.0, 0.0006, 0.3, 13000.0, 3000.0) {
}

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

void Catapult::updateTargeting(control::input::Controller *controller) {
  if (controller->l1Pressed()) {
    this->hold();
  } else if (controller->l2Pressed()) {
    this->launchRepeat();
  }
}

void Catapult::updateState() { //90 -> 20 (tgt)
  switch (this->state) {
  case HOLD: {
    const double update = -this->pid.update(2000, this->rotation.getRotation());
    logger::info("update {}", update);
    this->motor.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
    if (update == 0.0) {
      this->motor.brake();
    } else {
      this->motor.moveMillivolts(static_cast<int16_t>(update));
    }
  } break;
  case REPEAT_LAUNCH:
    this->motor.moveMillivolts(this->speed);
    break;
  case SINGLE_LAUNCH:
    this->motor.moveMillivolts(this->speed);
    break;
  }
  logger::info("CATA: {:.2f} @ {:.2f}", this->motor.getEfficiency(), this->rotation.getRotation() - 36000);
}
} // namespace robot
