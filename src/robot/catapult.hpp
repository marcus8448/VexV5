#ifndef ROBOT_CATAPULT_HPP
#define ROBOT_CATAPULT_HPP

#include "../control/pid.hpp"
#include "control/input/controller.hpp"
#include "device/motor.hpp"
#include "device/rotation.hpp"
#include "robot/device/pneumatics.hpp"

namespace robot {
constexpr uint16_t DEFAULT_CATAPULT_SPEED = 12000;

class Catapult {
  enum State { HOLD, REPEAT_LAUNCH, SINGLE_LAUNCH } state = HOLD;

  device::DirectMotor motor;
  device::Rotation rotation;
  device::PID pid;
  int16_t speed = 0;

public:
  explicit Catapult(int8_t motorPort, int8_t rotationPort);

  void launchOne(uint16_t speed = DEFAULT_CATAPULT_SPEED);
  void launchRepeat(uint16_t speed = DEFAULT_CATAPULT_SPEED);
  void hold();

  [[nodiscard]] int16_t getSpeed() const;

  void updateTargeting(control::input::Controller *controller);
  void updateState();
};
} // namespace robot
#endif // ROBOT_CATAPULT_HPP
