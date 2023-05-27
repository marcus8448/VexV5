#include "robot/claw.hpp"

#define CLOSED_POSITION 0.0
#define OPEN_POSITION 200.0

namespace robot {
Claw::Claw(uint8_t port) : motor(device::Motor(port, "Claw")) {}

void Claw::open() {
  if (this->state == CLOSED || this->state == CLOSING) {
    this->motor.moveAbsolute(OPEN_POSITION, 100);
    this->state = OPENING;
  }
}

void Claw::close() {
  if (this->state == OPEN || this->state == OPENING) {
    this->motor.moveAbsolute(CLOSED_POSITION, 100);
    this->state = CLOSING;
  }
}

Claw::ClawState Claw::getState() const { return this->state; }

void Claw::updateTargeting(control::input::Controller *controller) {
  if (controller->r1Pressed()) {
    this->close();
  } else if (controller->r2Pressed()) {
    this->open();
  }
}

void Claw::updateState() {
  if (this->state == OPENING) {
    if (std::abs(OPEN_POSITION - this->motor.getPosition()) < 1.0) {
      this->state = OPEN;
      this->motor.moveMillivolts(200);
    }
  } else if (this->state == CLOSING) {
    if (std::abs(CLOSED_POSITION - this->motor.getPosition()) < 1.0) {
      this->state = CLOSED;
      this->motor.moveMillivolts(-200);
    }
  }
}
} // namespace robot