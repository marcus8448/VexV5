#include "robot/device/pneumatics.hpp"
#include "pros/adi.hpp"
#include <cerrno>

namespace robot::device {
PneumaticPiston::PneumaticPiston(uint8_t port)
    : Device(port), piston(pros::ADIDigitalOut(port, false)), extended(false) {}
PneumaticPiston::PneumaticPiston(uint8_t port, bool defaultState)
    : Device(port), piston(pros::ADIDigitalOut(port, defaultState)), extended(defaultState) {}

void PneumaticPiston::extend() {
  if (!this->extended) {
    this->piston.set_value(true);
    this->extended = true;
  }
}

void PneumaticPiston::contract() {
  if (this->extended) {
    this->piston.set_value(false);
    this->extended = false;
  }
}

void PneumaticPiston::toggle() {
  if (this->extended) {
    this->contract();
  } else {
    this->extend();
  }
}

[[nodiscard]] bool PneumaticPiston::is_extended() { return this->extended; }

void PneumaticPiston::reconfigure() const {}

[[nodiscard]] bool PneumaticPiston::is_connected() const {
  bool b = this->piston.set_value(this->extended) == 1;
  errno = 0;
  return b;
}

[[nodiscard]] pros::ADIDigitalOut PneumaticPiston::get_raw_piston() const { return this->piston; }
} // namespace robot::device
