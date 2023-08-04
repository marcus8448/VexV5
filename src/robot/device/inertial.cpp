#include "robot/device/inertial.hpp"
#include "debug/error.hpp"
#include "debug/logger.hpp"
#include "pros/imu.h"
#include <cerrno>

namespace robot::device {
Inertial::Inertial(uint8_t port, const char *name) : Device("Inertial", name, port) {}

double Inertial::getRotation() const {
  if (pros::c::imu_get_status(this->port) == pros::E_IMU_STATUS_CALIBRATING) {
    error("Still calibrating!!");
  }
  return print_error(this->getName(), pros::c::imu_get_rotation(this->port));
}

double Inertial::getHeading() const {
  if (pros::c::imu_get_status(this->port) == pros::E_IMU_STATUS_CALIBRATING || get_error() == EAGAIN) {
    error("Still calibrating!!");
  }
  return print_error(this->getName(), pros::c::imu_get_heading(this->port));
}

double Inertial::getYaw() const { return pros::c::imu_get_yaw(this->port); }

double Inertial::getPitch() const { return pros::c::imu_get_pitch(this->port); }

double Inertial::getRoll() const { return pros::c::imu_get_roll(this->port); }

bool Inertial::isConnected() const {
  errno = 0;
  pros::imu_status_e d = pros::c::imu_get_status(this->port);
  int e = errno;
  errno = 0;
  return d == pros::imu_status_e::E_IMU_STATUS_ERROR || e == 0;
}

void Inertial::reconfigure() const {}

void Inertial::tare() { pros::c::imu_tare(this->port); }

void Inertial::calibrate() { pros::c::imu_reset_blocking(this->port); }
} // namespace robot::device
