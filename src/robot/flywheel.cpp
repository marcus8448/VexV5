#include "robot/flywheel.hpp"
#include "logger.hpp"

namespace robot {
Flywheel::Flywheel(pros::Motor *motor) : motor(motor) { motor->set_brake_mode(pros::E_MOTOR_BRAKE_COAST); }

Flywheel::~Flywheel() {
  free(this->motor);
  this->motor = nullptr;
}

void Flywheel::engage(double flywheelSpeed) {
  this->motor->move_velocity(-flywheelSpeed);
  this->engaged = true;
  this->speedFor = 0;
}

void Flywheel::spinUp(bool block) {
  this->motor->move_velocity(static_cast<int32_t>(MAX_SPEED));
  this->engaged = true;

  if (block) {
    this->waitForSpeed();
  }
}

void Flywheel::disengage() {
  this->engaged = false;
  this->speedFor = 0;
  this->motor->move(0);
  this->motor->brake();
}

[[nodiscard]] bool Flywheel::isEngaged() const { return this->engaged; }

void Flywheel::update(Controller *controller) {
  if (this->isEngaged()) {
    if (this->motor->get_target_velocity() != controller->flywheel_speed()) {
      this->engage(controller->flywheel_speed());
    }
    if (this->speedFor != -1) {
      if (this->speedFor == 10) {
        controller->rumble("-");
        controller->rumble("");
        this->speedFor = -1;
      }
      if (this->isUpToSpeed()) {
        this->speedFor++;
      } else {
        this->speedFor = 0;
      }
    } else {
      if (this->motor->get_actual_velocity() > -290.0) {
        this->speedFor = 0;
      }
    }
  }
  if (controller->r1_pressed()) {
    this->engage(controller->flywheel_speed());
  } else if (controller->r2_pressed()) {
    this->disengage();
  }
}

[[nodiscard]] pros::Motor *Flywheel::get_motor() const { return this->motor; }

double Flywheel::getVelocity() { return this->motor->get_actual_velocity(); }

bool Flywheel::isUpToSpeed() { return this->motor->get_actual_velocity() <= -DEFAULT_TARGET_SPEED; }

void Flywheel::waitForSpeed(int millis_timeout) {
  if (!this->engaged) {
    logger::warn("Waiting for flywheel to speed up while it's off!");
  }
  millis_timeout /= 50;
  int i = 0;
  while (this->motor->get_actual_velocity() > -DEFAULT_TARGET_SPEED) {
    if (i++ == millis_timeout) {
      break;
    }
    pros::delay(50);
  }
}
} // namespace robot
