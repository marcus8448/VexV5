#include "robot/expansion.hpp"
#include "logger.hpp"
#include "robot/device/motor.hpp"

namespace robot {
Expansion::Expansion(uint8_t port)
    : motor(device::Motor(port, pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_BRAKE, false)) {}

Expansion::~Expansion() = default;

void Expansion::launch() {
  logger::info("launching expansion");
  this->motor.move_absolute(0.0, 250.0);
  this->charged = false;
}

void Expansion::charge() {
  logger::info("charging expansion");
  this->motor.move_absolute(155.0, 25.0);
  this->charged = true;
}

[[nodiscard]] bool Expansion::has_launched() const { return !this->charged; }

void Expansion::update(Controller *controller) {
  if (controller->y_pressed()) {
    this->launch();
  }
  if (!this->charged && this->motor.get_position() < 0.0) {
    this->motor.stop();
  }
}

[[nodiscard]] device::Motor Expansion::get_motor() const { return this->motor; }
} // namespace robot
