#ifndef ROBOT_FLYWHEEL_HPP
#define ROBOT_FLYWHEEL_HPP

#include "robot/controller/controller.hpp"
#include "robot/device/motor.hpp"
#include <vector>

namespace robot {
/**
 * Represents the flywheel of the robot.
 */
class Flywheel : public Updatable {
private:
  enum State { IDLE, SPINNING_UP, SPINNING_DOWN, AT_SPEED };
  /**
   * The motor of the flywheel.
   */
  device::Motor primaryMotor;
  device::Motor secondaryMotor;

  int16_t targetMV = 0;
  std::vector<double> prevSpeeds = std::vector<double>();

  /**
   * Whether the flywheel is currently active.
   */
  State state = State::IDLE;

public:
  /**
   * Creates a new flywheel with the specified motor.
   * @param motor The motor of the flywheel.
   */
  explicit Flywheel(uint8_t port, uint8_t secondary_port);
  virtual ~Flywheel();

  /**
   * Engages the flywheel.
   * Sets the motor to run at the specified voltage.
   */
  void engage(int16_t flywheelMV, bool block = false);

  double wait_for_speed(double targetVelocity = -1.0, uint16_t millis_timeout = 5000);

  /**
   * Disengages the flywheel.
   * Stops providing power to run the motor. Does not immediately brake.
   */
  void disengage();

  void reset_speeds();

  [[nodiscard]] const device::Motor &get_primary_motor() const;
  [[nodiscard]] const device::Motor &get_secondary_motor() const;

  void update(Controller *controller) override;
};
} // namespace robot
#endif // ROBOT_FLYWHEEL_HPP
