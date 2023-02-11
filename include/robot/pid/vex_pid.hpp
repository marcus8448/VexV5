#ifndef ROBOT_PID_VEX_PID_HPP
#define ROBOT_PID_VEX_PID_HPP

#include "robot/pid/pid_controller.hpp"

namespace robot {
class VexPid : public PidController {
private:
  const uint8_t port;

public:
  explicit VexPid(uint8_t port);
  ~VexPid() override;

  void reset() override;
  void target_velocity(double targetVelocity) override;
  void update() override;
};
} // namespace robot
#endif // ROBOT_PID_VEX_PID_HPP
