#ifndef ROBOT_CATAPULT_HPP
#define ROBOT_CATAPULT_HPP

#include "control/input/controller.hpp"
#include "control/pid.hpp"
#include "device/motor.hpp"
#include "device/rotation.hpp"
#include "robot/device/pneumatics.hpp"

namespace robot {
constexpr uint16_t DEFAULT_CATAPULT_SPEED = 10000;

class Catapult {
public:
  uint16_t pendingLaunches = 0;
private:
  uint32_t delay = 0;
  uint32_t targetTime = 0;
  bool prime = true;

  enum Position { RELEASE, CHARGE } position = RELEASE;

  device::DirectMotor motor;
  device::DirectMotor motor2;
  device::Rotation rotation;
  device::PID pid;
  int16_t speed = 0;

public:
  explicit Catapult(int8_t motorPort, int8_t motor2Port, int8_t rotationPort);

  void launch(uint16_t count = 1, int16_t speed = DEFAULT_CATAPULT_SPEED, uint32_t delay = 500, bool wait = false);
  void hold();

  [[nodiscard]] int16_t getSpeed() const;

  void updateTargeting(control::input::Controller *controller);
  void updateState();
};
} // namespace robot
#endif // ROBOT_CATAPULT_HPP
