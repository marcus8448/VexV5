#include "robot/device/inertial.hpp"
#include "debug/error.hpp"
#include "debug/logger.hpp"
#include "pros/imu.h"
#include <cerrno>

namespace robot::device {
Inertial::Inertial(uint8_t port, const char *name) : Device(port, name) {
  pros::c::imu_reset(this->port);
}

double Inertial::get_heading() const {
  if (pros::c::imu_get_status(this->port) == pros::c::E_IMU_STATUS_CALIBRATING) {
    error("Still calibrating!!");
    check_error();
  }
  return print_error(pros::c::imu_get_heading(this->port));
}

double Inertial::get_yaw() const {
  return pros::c::imu_get_yaw(this->port);
}

double Inertial::get_pitch() const {
  return pros::c::imu_get_pitch(this->port);
}

double Inertial::get_roll() const {
  return pros::c::imu_get_roll(this->port);
}

bool Inertial::is_connected() const {
  errno = 0;
  pros::c::imu_status_e d = pros::c::imu_get_status(this->port);
  int e = errno;
  errno = 0;
  return d == pros::c::imu_status_e::E_IMU_STATUS_ERROR || e == 0;
}

void Inertial::update() {
}

void Inertial::reconfigure() const {}

void Inertial::tare() {
  pros::c::imu_tare(this->port);
}

void Inertial::calibrate() {
  pros::c::imu_reset_blocking(this->port);
}
}
