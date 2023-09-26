#ifndef ROBOT_INTAKE_HPP
#define ROBOT_INTAKE_HPP

#include "control/input/controller.hpp"
#include "device/motor.hpp"
#include "robot/device/pneumatics.hpp"

namespace robot {
constexpr uint16_t DEFAULT_INTAKE_SPEED = 12000;

class Intake {
private:
  device::DirectMotor leftMotor;
  device::DirectMotor rightMotor;
  device::PneumaticPiston pneumatic;

  int16_t speed = 0;

public:
  explicit Intake(int8_t leftPort, int8_t rightPort);

  void stop();
  void intake(uint16_t speed = DEFAULT_INTAKE_SPEED);
  void release(uint16_t speed = DEFAULT_INTAKE_SPEED);

  [[nodiscard]] int16_t getSpeed() const;

  void updateTargeting(control::input::Controller *controller);
  void updateState();
};
} // namespace robot
#endif // ROBOT_INTAKE_HPP
