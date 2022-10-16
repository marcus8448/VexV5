#ifndef ROBOT_INTAKE_HPP
#define ROBOT_INTAKE_HPP

#include "pros/motors.hpp"
#include "robot/controller/controller.hpp"

namespace robot {
/**
 * Represents the intake on the robot.
 */
class Intake : public Updatable {
private:
  /**
   * The motor of the intake.
   */
  pros::Motor *motor;

  /**
   * Whether the intake is currently running.
   */
  bool engaged = false;

public:
  /**
   * Creates a new intake with the specified motor.
   * @param motor the intake's motor.
   */
  explicit Intake(pros::Motor *motor);
  virtual ~Intake();

  /**
   * Engages the intake.
   * Runs the motor.
   */
  void engage();

  /**
   * Disengages the intake.
   * Stops providing power to run the motor. Does not immediately stop.
   */
  void disengage();

  /**
   * Returns whether the flywheel is currently engaged (running).
   * @return whether the flywheel is currently engaged (running).
   */
  [[nodiscard]] bool isEngaged() const;

  [[nodiscard]] pros::Motor *get_motor() const;

  void update(Controller *controller) override;
};
} // namespace robot
#endif // ROBOT_INTAKE_HPP