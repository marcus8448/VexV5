#ifndef ROBOT_FLYWHEEL_HPP
#define ROBOT_FLYWHEEL_HPP

#include "pros/motors.hpp"
#include "robot/controller/controller.hpp"

#define MAX_SPEED 600.0
#define DEFAULT_TARGET_SPEED 330.0

namespace robot {
/**
 * Represents the flywheel of the robot.
 */
class Flywheel : public Updatable {
private:
  /**
   * The motor of the flywheel.
   */
  pros::Motor *motor;
  /**
   * Whether the flywheel is currently active.
   */
  bool engaged = false;
  int8_t speedFor = 0;

public:
  /**
   * Creates a new flywheel with the specified motor.
   * @param motor The motor of the flywheel.
   */
  explicit Flywheel(pros::Motor *motor);
  virtual ~Flywheel();

  /**
   * Engages the flywheel.
   * Sets the motor to run at max speed.
   */
  void engage(double flywheelSpeed = 600.0);

  void spinUp(bool block = false);

  bool isUpToSpeed();

  void waitForSpeed(int millis_timeout = 5000);

  /**
   * Disengages the flywheel.
   * Stops providing power to run the motor. Does not immediately stop.
   */
  void disengage();

  /**
   * Returns the current velocity of the flywheel.
   * @return the current velocity of the flywheel.
   */
  double getVelocity();

  /**
   * Returns whether the flywheel is currently active.
   * @return whether the flywheel is currently active.
   */
  [[nodiscard]] bool isEngaged() const;

  [[nodiscard]] pros::Motor *get_motor() const;

  void update(Controller *controller) override;
};
} // namespace robot
#endif // ROBOT_FLYWHEEL_HPP
