#ifndef ROBOT_ARM_HPP
#define ROBOT_ARM_HPP

#include "control/input/controller.hpp"
#include "device/motor.hpp"

namespace robot {
class Arm {
public:
  enum ArmState { NONE, RAISING, LOWERING };

private:
  ArmState state = NONE;

public:
  device::Motor primary;
  device::Motor secondary;

  explicit Arm(uint8_t primaryPort, uint8_t secondaryPort);

  void raise();
  void lower();

  [[nodiscard]] ArmState getState() const;

  void updateTargeting(control::input::Controller *controller);
  void updateState();
};
} // namespace robot
#endif // ROBOT_ARM_HPP
