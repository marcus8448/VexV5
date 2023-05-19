#ifndef ROBOT_PID_VEX_HPP
#define ROBOT_PID_VEX_HPP

#include "pid_controller.hpp"

namespace robot {
class Vex : public VelocityPid {
private:
  const uint8_t port;

public:
  explicit Vex(uint8_t port);
  ~Vex() override;

  void reset() override;
  void startTargeting(double targetVelocity) override;
  void update() override;
};
} // namespace robot
#endif // ROBOT_PID_VEX_HPP
