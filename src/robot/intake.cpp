#include "robot/intake.hpp"

namespace robot {

Intake::Intake(pros::Motor *motor) : motor(motor) { motor->set_brake_mode(pros::E_MOTOR_BRAKE_COAST); }

Intake::~Intake() {
  free(this->motor);
  this->motor = nullptr;
}

void Intake::engage() {
  this->motor->move(95);
  this->engaged = true;
}

void Intake::disengage() {
  this->engaged = false;
  this->motor->brake();
}

[[nodiscard]] bool Intake::isEngaged() const { return this->engaged; }

void Intake::update(Controller *controller) {
  if (controller->r1_pressed()) {
    this->engage();
  } else if (controller->r2_pressed()) {
    this->disengage();
  }
}
} // namespace robot