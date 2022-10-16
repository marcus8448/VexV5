#include "robot/indexer.hpp"

namespace robot {
Indexer::Indexer(pros::Motor *motor) : motor(motor) { motor->set_brake_mode(pros::E_MOTOR_BRAKE_COAST); }

Indexer::~Indexer() {
  free(this->motor);
  this->motor = nullptr;
}

void Indexer::push() {
  if (!reset) {
    this->motor->move_absolute(90.0, 150);
  }
}

void Indexer::update(Controller *controller) {
  if (controller->up_pressed()) {
    this->push();
  }
  if (this->motor->get_target_position() == 90.0 && this->motor->get_position() >= 89) {
    reset = true;
    this->motor->move_absolute(0.0, 150);
  }
}

[[nodiscard]] pros::Motor *Indexer::get_motor() const {
  return this->motor;
}
} // namespace robot
