#ifndef ROBOT_PID_PID_CONTROLLER_HPP
#define ROBOT_PID_PID_CONTROLLER_HPP

#include <cstdint>

namespace robot {
class PidController {
public:
  virtual ~PidController() = default;

  virtual void reset() = 0;
  virtual void target_velocity(double targetVelocity) = 0;
  virtual void update() = 0;
};
}
#endif // ROBOT_PID_PID_CONTROLLER_HPP
