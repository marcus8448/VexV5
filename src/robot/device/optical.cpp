#include "pros/optical.hpp"
#include "robot/device/optical.hpp"
#include <cerrno>
#include <cstdint>

namespace robot::device {
ColourSensor::ColourSensor(uint8_t port) : optical(pros::Optical(port)), port(port) { this->optical.disable_gesture(); }

double ColourSensor::get_hue() { return this->optical.get_hue(); }

double ColourSensor::get_saturation() { return this->optical.get_saturation(); }

double ColourSensor::get_brightness() { return this->optical.get_brightness(); }

int32_t ColourSensor::get_proximity() { return this->optical.get_proximity(); }

int32_t ColourSensor::set_led_pwm(uint8_t value) { return this->optical.set_led_pwm(value); }

int32_t ColourSensor::get_led_pwm() { return this->optical.get_led_pwm(); }

bool ColourSensor::disable_gesture() { return this->optical.disable_gesture(); }

bool ColourSensor::is_connected() { return this->optical.get_proximity() == INT32_MAX && errno == ENODEV; }

uint8_t ColourSensor::get_port() const { return this->port; }
} // namespace robot::device
