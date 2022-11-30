#include "robot/expansion.hpp"
#include "logger.hpp"
#include "robot/device/motor.hpp"
#include "robot/device/pneumatics.hpp"

namespace robot {
Expansion::Expansion(uint8_t port) : piston(device::PneumaticPiston(port)) {}

Expansion::~Expansion() = default;

void Expansion::launch() {
  if (!this->launched) {
    logger::info("Launching expansion");
    this->piston.extend();
    this->launched = true;
  }
}

[[nodiscard]] bool Expansion::has_launched() const { return this->launched; }

void Expansion::update(Controller *controller) {
  if (!this->launched) {
    if (controller->up_pressed() % 5 == 1) {
      controller->rumble("-");
    } else if (controller->up_pressed() > 25) {
      this->launch();
    }
  }
}

[[nodiscard]] const device::PneumaticPiston &Expansion::get_piston() const { return this->piston; }
} // namespace robot
