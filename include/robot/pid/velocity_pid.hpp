#ifndef ROBOT_PID_VELOCITY_PID_HPP
#define ROBOT_PID_VELOCITY_PID_HPP

#include "robot/pid/pid_controller.hpp"

namespace robot {
class VelocityPid: public PidController {
private:
  double error[3];
  double proportional = 0.0;

  double integral = 0.0;
  double derivative = 0.0;

  double d0 = 0.0;
  double d1 = 0.0;
  double fd0 = 0.0;
  double fd1 = 0.0;

  double targetVelocity = 0.0;
  double output = 0.0;

  uint16_t count = 0;

  const uint8_t port;

  const double Kp;
  const double Ki;
  const double Kd;
  const double dt;
  const double A0;
  const double A1;
  const double A0d;
  const double A1d;
  const double A2d;
  const double tau;
  const double alpha;

public:
  explicit VelocityPid(uint8_t port, double Kp, double Ki, double Kd, double dt);
  ~VelocityPid() override;

  void reset() override;
  void target_velocity(double targetVelocity) override;
  void update() override;
};
}
#endif // ROBOT_PID_VELOCITY_PID_HPP
