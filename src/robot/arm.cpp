#include "robot/arm.hpp"

namespace robot {
Arm::Arm(int8_t primaryPort, int8_t secondaryPort)
    : primary(device::PneumaticPiston(primaryPort, "Arm 1")),
      secondary(device::PneumaticPiston(secondaryPort, "Arm 2")) {}

void Arm::open() {
  if (this->state != OPEN) {
    this->state = OPEN;
    this->primary.extend();
    this->secondary.extend();
  }
}

void Arm::close() {
  if (this->state != CLOSED) {
    this->state = CLOSED;
    this->primary.contract();
    this->secondary.contract();
  }
}

Arm::State Arm::getState() const { return this->state; }

void Arm::updateTargeting(control::input::Controller *controller) {
  if (controller->l2Pressed()) {
    this->close();
  } else if (controller->l1Pressed()) {
    this->open();
  }
}
} // namespace robot
