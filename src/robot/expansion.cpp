#include "robot/expansion.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"

namespace robot {
Expansion::Expansion(pros::Motor *motor) : motor(motor) { motor->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); }

Expansion::~Expansion() {
  free(this->motor);
  this->motor = nullptr;
}

void Expansion::launch() {
  logger::info("launching expansion");
  motor->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  this->motor->move_absolute(0.0, 250.0);
  this->charged = false;
}

void Expansion::charge() {
  logger::info("charging expansion");
  motor->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  this->motor->move_absolute(155.0, 25.0);
  this->charged = true;
}

[[nodiscard]] bool Expansion::has_launched() const { return !this->charged; }

void Expansion::update(Controller *controller) {
  if (controller->y_pressed()) {
    this->launch();
  }
  if (!this->charged && this->motor->get_position() < 0.0) {
    this->motor->move(0);
    this->motor->brake();
  }
}

[[nodiscard]] pros::Motor *Expansion::get_motor() const { return this->motor; }
} // namespace robot
