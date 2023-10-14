#ifndef ROBOT_DEVICE_PID_HPP
#define ROBOT_DEVICE_PID_HPP

#include "debug/logger.hpp"
#include "device.hpp"
#include "pros/motors.h"
#include "robot/device/motor.hpp"

namespace robot::device {
class PID {
public:
  double kp;
  double ki;
  double kd;
  double integralRange;
  double acceptableError;
  double moveMin = 0.0;
  double output = 0.0;

private:
  double error = 0.0;
  double prevError = 0.0;
  double integral = 0.0;

public:
  explicit PID(double Kp, double Ki, double Kd, double integralRange, double acceptableError);
  explicit PID();
  PID& operator=(const PID &pid);
  void resetState();

  [[nodiscard]] double getError() const;

  double update(double target, double value);
};
} // namespace robot::device
#endif // ROBOT_DEVICE_PID_HPP
