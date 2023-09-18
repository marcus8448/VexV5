#ifndef ROBOT_ARM_HPP
#define ROBOT_ARM_HPP

#include "control/input/controller.hpp"
#include "device/pneumatics.hpp"

namespace robot {
class Arm {
public:
  enum State { CLOSED, OPEN };

private:
  device::PneumaticPiston primary;
  device::PneumaticPiston secondary;

  State state = CLOSED;

public:
  explicit Arm(int8_t primaryPort, int8_t secondaryPort);

  void open();
  void close();

  [[nodiscard]] State getState() const;

  void updateTargeting(control::input::Controller *controller);
};
} // namespace robot
#endif // ROBOT_ARM_HPP
