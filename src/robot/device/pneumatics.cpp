#include "robot/device/pneumatics.hpp"
#include "debug/error.hpp"
#include "pros/adi.h"

namespace robot::device {
PneumaticPiston::PneumaticPiston(int8_t port, const char *name, bool defaultState)
    : Device("Piston", name, port), extended(defaultState) {
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

[[nodiscard]] bool PneumaticPiston::isExtended() const { return this->extended; }

void PneumaticPiston::reconfigure() const { pros::c::adi_digital_write(this->port, this->extended); }

[[nodiscard]] bool PneumaticPiston::isConnected() const {
  if (error::check(pros::c::adi_digital_write(this->port, this->extended))) {
    return !error::isDisconnected();
  }
  return true;
}
} // namespace robot::device
