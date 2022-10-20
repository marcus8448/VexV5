#include "robot/intake.hpp"

namespace robot {

Intake::Intake(pros::Motor *motor) : motor(motor) { motor->set_brake_mode(pros::E_MOTOR_BRAKE_COAST); }

Intake::~Intake() {
  free(this->motor);
  this->motor = nullptr;
}

void Intake::engage() {
  this->motor->move(-127);
  this->engaged = true;
}

void Intake::reverse() {
  this->motor->move(127);
  this->engaged = true;
}

void Intake::disengage() {
  this->engaged = false;
  this->motor->brake();
}

[[nodiscard]] bool Intake::isEngaged() const { return this->engaged; }

[[nodiscard]] pros::Motor *Intake::get_motor() const { return this->motor; }

void Intake::update(Controller *controller) {
  if (controller->l1_pressed() && controller->l2_pressed()) {
    this->reverse();
  } else if (controller->l1_pressed()) {
    this->engage();
  } else if (controller->l2_pressed()) {
    this->disengage();
  }
}
} // namespace robot
