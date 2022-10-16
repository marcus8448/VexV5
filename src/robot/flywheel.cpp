#include "robot/flywheel.hpp"
#include "logger.hpp"

namespace robot {
Flywheel::Flywheel(pros::Motor *motor) : motor(motor) { motor->set_brake_mode(pros::E_MOTOR_BRAKE_COAST); }

Flywheel::~Flywheel() {
  free(this->motor);
  this->motor = nullptr;
}

void Flywheel::engage() {
  this->motor->move(127);
  this->engaged = true;
}

void Flywheel::engage(double targetSpeed, bool block) {
  this->motor->move_velocity(static_cast<int32_t>(targetSpeed));
  this->engaged = true;

  if (block) {
    this->waitForSpeed(targetSpeed - 20.0);
  }
}

void Flywheel::disengage() {
  this->engaged = false;
  this->motor->brake();
}

[[nodiscard]] bool Flywheel::isEngaged() const { return this->engaged; }

void Flywheel::update(Controller *controller) {
  if (controller->r1_pressed()) {
    this->engage();
  } else if (controller->r2_pressed()) {
    this->disengage();
  }
}

[[nodiscard]] pros::Motor *Flywheel::get_motor() const {
  return this->motor;
}

double Flywheel::getVelocity() { return this->motor->get_actual_velocity(); }

bool Flywheel::isUpToSpeed(double velocity) {
  return this->motor->get_actual_velocity() >= velocity;
}

void Flywheel::waitForSpeed(double velocity, int millis_timeout) {
  if (!this->engaged) {
    logger::warn("Waiting for flywheel to speed up while it's off!");
  }
  millis_timeout /= 50;
  int i = 0;
  while (this->motor->get_actual_velocity() < velocity) {
    if (i++ == millis_timeout) {
      break;
    }
    pros::delay(50);
  }
}
} // namespace robot
