#ifndef ROBOT_CATAPULT_HPP
#define ROBOT_CATAPULT_HPP

#include "control/input/controller.hpp"
#include "device/motor.hpp"
#include "robot/device/pneumatics.hpp"

namespace robot {
constexpr uint16_t DEFAULT_CATAPULT_SPEED = 12000;

class Catapult {
  enum State { RELEASE, HOLD, REPEAT_LAUNCH, SINGLE_LAUNCH } state = RELEASE;

  device::DirectMotor motor;
  int16_t speed = 0;

public:
  explicit Catapult(int8_t motorPort);

  void stop();
  void launchOne(uint16_t speed = DEFAULT_CATAPULT_SPEED);
  void launchRepeat(uint16_t speed = DEFAULT_CATAPULT_SPEED);
  void release();
  void hold();

  [[nodiscard]] int16_t getSpeed() const;

  void updateTargeting(control::input::Controller *controller);
  void updateState();
};
} // namespace robot
#endif // ROBOT_CATAPULT_HPP
