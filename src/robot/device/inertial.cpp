#include "robot/device/inertial.hpp"
#include "debug/error.hpp"
#include "debug/logger.hpp"
#include "pros/imu.h"

namespace robot::device {
Inertial::Inertial(int8_t port, const char *name) : Device("Inertial", name, port) { this->calibrate(); }

double Inertial::getRotation() const {
  if (this->isCalibrating()) {
    logger::error("Still calibrating!!");
    return 0.0;
  }
  return error::print(this->getName(), pros::c::imu_get_rotation(this->port));
}

double Inertial::getHeading() const {
  if (this->isCalibrating()) {
    logger::error("Still calibrating!!");
    return 0.0;
  }
  double heading = pros::c::imu_get_heading(this->port);
  if (error::check(heading)) {
    error::print(this->getName());
    return 0.0;
  }
  return heading;
}

double Inertial::getYaw() const { return pros::c::imu_get_yaw(this->port); }

double Inertial::getPitch() const { return pros::c::imu_get_pitch(this->port); }

double Inertial::getRoll() const { return pros::c::imu_get_roll(this->port); }

bool Inertial::isCalibrating() const {
  pros::imu_status_e_t status = pros::c::imu_get_status(this->port);
  return status == pros::E_IMU_STATUS_CALIBRATING || status == pros::E_IMU_STATUS_ERROR;
}

bool Inertial::isConnected() const {
  return pros::c::imu_get_status(this->port) == pros::E_IMU_STATUS_ERROR;
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
