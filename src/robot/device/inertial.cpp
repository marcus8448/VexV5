#include "robot/device/inertial.hpp"
#include "debug/error.hpp"
#include "debug/logger.hpp"
#include "pros/imu.h"
#include <cerrno>

namespace robot::device {
Inertial::Inertial(uint8_t port, const char *name) : Device("Inertial", name, port) { pros::c::imu_reset(this->port); }

double Inertial::getHeading() const {
  if (pros::c::imu_get_status(this->port) == pros::c::E_IMU_STATUS_CALIBRATING) {
    error("Still calibrating!!");
    check_error();
  }
  return print_error(pros::c::imu_get_heading(this->port));
}

double Inertial::getYaw() const { return pros::c::imu_get_yaw(this->port); }

double Inertial::getPitch() const { return pros::c::imu_get_pitch(this->port); }

double Inertial::getRoll() const { return pros::c::imu_get_roll(this->port); }

bool Inertial::isConnected() const {
  errno = 0;
  pros::c::imu_status_e d = pros::c::imu_get_status(this->port);
  int e = errno;
  errno = 0;
  return d == pros::c::imu_status_e::E_IMU_STATUS_ERROR || e == 0;
}

void Inertial::reconfigure() const {}

void Inertial::tare() { pros::c::imu_tare(this->port); }

void Inertial::calibrate() { pros::c::imu_reset_blocking(this->port); }
} // namespace robot::device
