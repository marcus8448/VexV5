#include "robot/device/pneumatics.hpp"
#include "pros/adi.h"
#include <cerrno>

namespace robot::device {
PneumaticPiston::PneumaticPiston(uint8_t port, const char *name, bool defaultState)
    : Device(port, name), extended(defaultState) {
  pros::c::adi_port_set_config(this->port, pros::adi_port_config_e_t::E_ADI_DIGITAL_OUT);
  pros::c::adi_digital_write(this->port, this->extended);
}

void PneumaticPiston::extend() {
  if (!this->extended) {
    pros::c::adi_digital_write(this->port, true);
    this->extended = true;
  }
}

void PneumaticPiston::contract() {
  if (this->extended) {
    pros::c::adi_digital_write(this->port, false);
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

void PneumaticPiston::update() {}

[[nodiscard]] bool PneumaticPiston::is_extended() const { return this->extended; }

void PneumaticPiston::reconfigure() const {}

[[nodiscard]] bool PneumaticPiston::is_connected() const {
  bool b = pros::c::adi_digital_write(this->port, this->extended) == 1;
  errno = 0;
  return b;
}
} // namespace robot::device
