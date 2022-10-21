#ifndef ROBOT_INTAKE_HPP
#define ROBOT_INTAKE_HPP

#include "configuration.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "robot/controller/controller.hpp"

namespace robot {
/**
 * Represents the intake on the robot.
 * Also controls the roller.
 */
class Intake : public Updatable {
private:
  /**
   * The motor of the intake/roller.
   */
  pros::Motor *motor;
  pros::Optical *optical;

  /**
   * Whether the intake is currently running.
   */
  bool engaged = false;

public:
  /**
   * Creates a new intake with the specified motor.
   * @param motor the intake's motor.
   */
  explicit Intake(pros::Motor *motor, pros::Optical *optical);
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

  bool looking_at_roller();

  void spin_until_colour(config::AllianceColour teamColour, uint32_t timeout);

  /**
   * Returns whether the intake is currently engaged (running).
   * @return whether the intake is currently engaged (running).
   */
  [[nodiscard]] bool isEngaged() const;

  [[nodiscard]] pros::Motor *get_motor() const;

  void update(Controller *controller) override;
  void reverse();
};
} // namespace robot
#endif // ROBOT_INTAKE_HPP
