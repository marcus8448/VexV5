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

void Indexer::cycle() {
  if (this->reset) {
    this->motor->move_absolute(0.0, 150);
    this->awaitReady();
  }

  this->motor->move_absolute(90.0, 150);
  this->awaitPush();
}

void Indexer::awaitReady(int millis_timeout) {
  if (this->reset) {
    millis_timeout /= 50;
    int i = 0;
    while (this->motor->get_position() > 1.0) {
      if (i++ == millis_timeout) {
        break;
      }
      pros::delay(50);
    }
    this->motor->brake();
    this->reset = false;
  }
}

void Indexer::awaitPush(int millis_timeout) {
  if (!this->reset) {
    millis_timeout /= 50;
    int i = 0;
    while (this->motor->get_position() < 89.0) {
      if (i++ == millis_timeout) {
        break;
      }
      pros::delay(50);
    }
    this->reset = true;
  }
}

void Indexer::update(Controller *controller) {
  if (controller->up_pressed()) {
    this->push();
  }
  if (this->reset) {
    this->motor->move_absolute(0.0, 150);
    if (this->motor->get_position() < 1.0) {
      this->motor->brake();
      this->reset = false;
    }
  } else {
    if (this->motor->get_target_position() == 90.0 && this->motor->get_position() >= 89.0) {
      this->reset = true;
    }
  }
}

[[nodiscard]] pros::Motor *Indexer::get_motor() const { return this->motor; }
} // namespace robot
