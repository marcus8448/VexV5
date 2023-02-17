#ifndef ROBOT_PID_TBH_PID_HPP
#define ROBOT_PID_TBH_PID_HPP

#include "robot/pid/pid_controller.hpp"

namespace robot {
class TBHControl : public PidController {
private:
  double target = 0.0;
  double output = 0.0;
  double prevError = 0.0;
  double tbh = 0.0;

  const uint8_t port;
  const uint8_t port2;
  const double gain; // out += diff in velocity * gain

public:
  explicit TBHControl(uint8_t port, uint8_t port2, double gain);
  ~TBHControl() override;

  void reset() override;
  void target_velocity(double targetVelocity) override;
  void update() override;
};
} // namespace robot
#endif // ROBOT_PID_TBH_PID_HPP
