#include "pros/optical.h"
#include "robot/device/optical.hpp"
#include <cerrno>

namespace robot::device {
Optical::Optical(int8_t port, const char *name, uint8_t led_pwm, bool gesture)
    : Device("Optical", name, port), led_pwm(led_pwm), gesture(gesture) {
  if (gesture) {
    pros::c::optical_enable_gesture(this->port);
  } else {
    pros::c::optical_disable_gesture(this->port);
  }
  pros::c::optical_set_led_pwm(this->port, this->led_pwm);
}

double Optical::getHue() const { return pros::c::optical_get_hue(this->port); }

double Optical::getSaturation() const { return pros::c::optical_get_saturation(this->port); }

double Optical::getBrightness() const { return pros::c::optical_get_brightness(this->port); }

int32_t Optical::getProximity() const { return pros::c::optical_get_proximity(this->port); }

void Optical::setLedPwm(uint8_t value) {
  if (this->led_pwm != value) {
    this->led_pwm = value;
    pros::c::optical_set_led_pwm(this->port, value);
  }
}

int32_t Optical::getLedPwm() const { return pros::c::optical_get_led_pwm(this->port); }

void Optical::enableGesture() {
  if (!this->gesture) {
    this->gesture = true;
    pros::c::optical_enable_gesture(this->port);
  }
}

void Optical::disableGesture() {
  if (this->gesture) {
    this->gesture = false;
    pros::c::optical_disable_gesture(this->port);
  }
}

void Optical::reconfigure() const {
  if (this->gesture) {
    pros::c::optical_enable_gesture(this->port);
  } else {
    pros::c::optical_disable_gesture(this->port);
  }
}

bool Optical::isConnected() const {
  errno = 0;
  return pros::c::optical_get_proximity(port) != INT32_MAX && checkConnect();
}
} // namespace robot::device
