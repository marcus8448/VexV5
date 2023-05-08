#ifndef ROBOT_PID_PID_CONTROLLER_HPP
#define ROBOT_PID_PID_CONTROLLER_HPP

#include <cstdint>

namespace robot {
class VelocityPid {
public:
  virtual ~VelocityPid() = default;

  virtual void reset() = 0;
  virtual void startTargeting(double velocity) = 0;
  virtual void update() = 0;
};
}
#endif // ROBOT_PID_PID_CONTROLLER_HPP
