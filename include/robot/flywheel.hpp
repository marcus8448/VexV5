#ifndef ROBOT_FLYWHEEL_HPP
#define ROBOT_FLYWHEEL_HPP

#include "pros/motors.hpp"
#include "robot/controller/controller.hpp"

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
  void engage();

  /**
   * Disengages the flywheel.
   * Stops providing power to run the motor. Does not immediately stop.
   */
  void disengage();

  /**
   * Returns the current velocity of the flywheel.
   * @return the current velocity of the flywheel.
   */
  double get_velocity();

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
