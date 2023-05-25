#ifndef ROBOT_CLAW_HPP
#define ROBOT_CLAW_HPP

#include "control/input/controller.hpp"
#include "device/motor.hpp"

namespace robot {
class Claw {
public:
  enum ClawState {
    OPENING,
    OPEN,
    CLOSING,
    CLOSED
  };

private:
  ClawState state = CLOSED;
public:
  device::Motor motor;

  explicit Claw(uint8_t port);

  void open();
  void close();

  [[nodiscard]] ClawState getState() const;

  void updateTargeting(control::input::Controller *controller);
  void updateState();
};
}
#endif // ROBOT_CLAW_HPP
