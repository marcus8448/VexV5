#ifndef ROBOT_FLYWHEEL_HPP
#define ROBOT_FLYWHEEL_HPP

#include "robot/controller/controller.hpp"
#include "robot/device/motor.hpp"

#define FLYWHEEL_TARGET_SPEED 300

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
  device::Motor first_motor;
  device::Motor second_motor;
  /**
   * Whether the flywheel is currently active.
   */
  State state = State::IDLE;

public:
  /**
   * Creates a new flywheel with the specified motor.
   * @param motor The motor of the flywheel.
   */
  explicit Flywheel(uint8_t port, uint8_t secondary_port = 99);
  virtual ~Flywheel();

  /**
   * Engages the flywheel.
   * Sets the motor to run at max speed.
   */
  void engage(int32_t flywheelSpeed = FLYWHEEL_TARGET_SPEED, bool block = false);

  bool is_up_to_speed();

  void wait_for_speed(uint16_t millis_timeout = 5000);

  /**
   * Disengages the flywheel.
   * Stops providing power to run the motor. Does not immediately stop.
   */
  void disengage();

  /**
   * Returns the current velocity of the flywheel.
   * @return the current velocity of the flywheel.
   */
  double get_first_motor_velocity();
  double get_second_motor_velocity();

  [[nodiscard]] const device::Motor &get_first_motor() const;
  [[nodiscard]] const device::Motor &get_second_motor() const;

  void update(Controller *controller) override;
};
} // namespace robot
#endif // ROBOT_FLYWHEEL_HPP
