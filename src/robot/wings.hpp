#ifndef ROBOT_WINGS_HPP
#define ROBOT_WINGS_HPP

#include "control/input/controller.hpp"
#include "robot/device/pneumatics.hpp"

namespace robot {
constexpr uint16_t DEFAULT_WINGS_SPEED = 12000;

class Wings {
  device::PneumaticPiston left;
  device::PneumaticPiston right;

  bool leftOpen = false;
  bool rightOpen = false;

public:
  explicit Wings(int8_t leftPneumatic, int8_t rightPneumatic);

  void open();
  void openLeft();
  void openRight();

  void close();
  void closeLeft();
  void closeRight();

  void updateTargeting(control::input::Controller *controller);
};
} // namespace robot
#endif // ROBOT_WINGS_HPP
