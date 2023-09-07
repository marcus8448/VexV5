#include "robot/arm.hpp"

namespace robot {
Arm::Arm(int8_t primaryPort, int8_t secondaryPort)
    : primary(device::Motor(primaryPort, "Arm 1")), secondary(device::Motor(secondaryPort, "Arm 2")) {
  this->primary.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  this->secondary.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
}

void Arm::raise() { this->state = RAISING; }

void Arm::lower() { this->state = LOWERING; }

Arm::ArmState Arm::getState() const { return this->state; }

void Arm::updateTargeting(control::input::Controller *controller) {
  if (controller->l2Pressed()) {
    this->lower();
  } else if (controller->l1Pressed()) {
    this->raise();
  } else {
    this->state = NONE;
  }
}

void Arm::updateState() {
  if (this->state == RAISING) {
    this->primary.moveMillivolts(11500);
    this->secondary.moveMillivolts(11500);
  } else if (this->state == LOWERING) {
    this->primary.moveMillivolts(-11500);
    this->secondary.moveMillivolts(-11500);
  } else {
    this->primary.brake();
    this->secondary.brake();
  }
}
} // namespace robot