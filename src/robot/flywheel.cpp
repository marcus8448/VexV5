#include "robot/flywheel.hpp"

namespace robot {
Flywheel::Flywheel(pros::Motor *motor) : motor(motor) {
  motor->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

void Flywheel::engage() {
  this->motor->move(-127);
  this->engaged = true;
}

void Flywheel::disengage() {
  this->engaged = false;
  this->motor->move(0);
}

bool Flywheel::isEngaged() {
  return this->engaged;
}

void Flywheel::update(Controller *controller) {
  if (controller->r1_pressed()) {
    this->engage();
  } else if (controller->r2_pressed()) {
    this->disengage();
  }
}
}
