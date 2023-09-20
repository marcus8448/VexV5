#include "robot/device/inertial.hpp"
#include "debug/error.hpp"
#include "debug/logger.hpp"
#include "pros/imu.h"
#include <cerrno>

namespace robot::device {
Inertial::Inertial(int8_t port, const char *name) : Device("Inertial", name, port) { this->calibrate(); }

double Inertial::getRotation() const {
  if (this->isCalibrating()) {
    logger::error("Still calibrating!!");
    return 0.0;
  }
  return print_error(this->getName(), pros::c::imu_get_rotation(this->port));
}

double Inertial::getHeading() const {
  if (this->isCalibrating()) {
    logger::error("Still calibrating!!");
    return 0.0;
  }
  return print_error(this->getName(), pros::c::imu_get_heading(this->port));
}

double Inertial::getYaw() const { return pros::c::imu_get_yaw(this->port); }

double Inertial::getPitch() const { return pros::c::imu_get_pitch(this->port); }

double Inertial::getRoll() const { return pros::c::imu_get_roll(this->port); }

bool Inertial::isCalibrating() const {
  return pros::c::imu_get_status(this->port) == pros::E_IMU_STATUS_CALIBRATING || get_error() == EAGAIN;
}

bool Inertial::isConnected() const {
  errno = 0;
  pros::c::imu_get_status(this->port);

  int e = errno;
  errno = 0;
  return e != ENODEV;
}

void Inertial::reconfigure() const {}

void Inertial::tare() { pros::c::imu_tare(this->port); }

void Inertial::calibrate() {
  if (this->isConnected()) {
    logger::scope("Calibrate IMU");
    pros::c::imu_reset_blocking(this->port);
    logger::endScope();
  } else {
    logger::warn("IMU not connected");
  }
}
} // namespace robot::device
