#include "robot/device/pneumatics.hpp"
#include "pros/adi.hpp"

namespace robot::device {
PneumaticPiston::PneumaticPiston(char port) : port(port), piston(pros::ADIDigitalOut(port, false)), extended(false) {}
PneumaticPiston::PneumaticPiston(char port, bool defaultState)
    : port(port), piston(pros::ADIDigitalOut(port, defaultState)), extended(defaultState) {}

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

[[nodiscard]] uint8_t PneumaticPiston::get_port() const { return this->port; }

[[nodiscard]] bool PneumaticPiston::is_connected() { return this->piston.set_value(this->extended) == 1; }

[[nodiscard]] pros::ADIDigitalOut PneumaticPiston::get_raw_piston() const { return this->piston; }
} // namespace robot::device
