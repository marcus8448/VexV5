#include "pid.hpp"
#include "debug/error.hpp"
#include "debug/logger.hpp"
#include "robot/device/motor.hpp"

#include <cmath>

namespace robot::device {
static double clampMv(double value, double moveMin);

PID::PID(double Kp, double Ki, double Kd, double integralRange, double acceptableError)
    : kp(Kp), ki(Ki), kd(Kd), integralRange(integralRange), acceptableError(acceptableError) {}

PID::PID() : kp(0.0), ki(0.0), kd(0.0), integralRange(0.0), acceptableError(0.0) {}

PID &PID::operator=(const PID &pid) {
  this->kp = pid.kp;
  this->ki = pid.ki;
  this->kd = pid.kd;
  this->integralRange = pid.integralRange;
  this->acceptableError = pid.acceptableError;
  return *this;
}

void PID::resetState() {
  this->integral = 0;
  this->error = 0;
}

double PID::update(double target, double value) {
  this->error = target - value;
  if (this->error > 0 != this->prevError > 0 || std::abs(this->error) > this->integralRange) {
    this->integral = 0;
  }

  if (std::abs(this->error) < this->acceptableError || error::check(value)) {
    this->integral = 0;
    this->prevError = this->error;
    return this->output = 0.0;
  }

  this->integral += this->error;
  this->output = clampMv(
      this->error * this->kp + this->integral * this->ki + (this->error - this->prevError) * this->kd, this->moveMin);
  logger::debug("{:.2f} {:.2f}/{:.2f}, {:.3f} {:.3f} {:.3f} * {:.2f} {:.2f} {:.2f} -> {:.2f}/{:.2f}/{:.2f} -> {:.2f}",
               this->error, value, target, this->kp, this->ki, this->kd, this->error, this->integral,
               this->error - this->prevError, this->error * this->kp, this->integral * this->ki,
               (this->error - this->prevError) * this->kd, this->output);

  this->prevError = this->error;
  return this->output;
}

double PID::getError() const { return error; }

static double clampMv(double value, double moveMin) {
  //  if (std::abs(value) < moveMin) {
  //    value = value < 0 ? -moveMin : moveMin;
  //  }
  return value > Motor::MAX_MILLIVOLTS    ? Motor::MAX_MILLIVOLTS
         : value < -Motor::MAX_MILLIVOLTS ? -Motor::MAX_MILLIVOLTS
                                          : value;
}
} // namespace robot::device
