#include "robot/device/gyro.hpp"
#include "pros/adi.hpp"
#include <cerrno>

namespace robot::device {
Gyro::Gyro(uint8_t port, const char *name) : Device(port, name), handle(pros::c::adi_gyro_init(port, 1.0)) {}

double Gyro::get_rotation() const { return pros::c::adi_gyro_get(this->handle) / 10.0; }

bool Gyro::is_connected() const {
  errno = 0;
  pros::c::adi_gyro_get(this->handle);
  return errno == 0;
}

void Gyro::update() {}

void Gyro::reconfigure() const {}
} // namespace robot::device
