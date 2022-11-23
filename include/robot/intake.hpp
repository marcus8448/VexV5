#ifndef ROBOT_INTAKE_HPP
#define ROBOT_INTAKE_HPP

#include "robot/device/optical.hpp"
#define INTAKE_POWER_LEVEL 11000

#include "configuration.hpp"
#include "pros/optical.hpp"
#include "robot/controller/controller.hpp"
#include "robot/device/motor.hpp"

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
  device::Motor motor;
  device::ColourSensor colour;

  /**
   * Whether the intake is currently running.
   */
  bool engaged = false;

public:
  /**
   * Creates a new intake with the specified motor.
   * @param motor the intake's motor.
   */
  explicit Intake(uint8_t motorPort, uint8_t colourPort);
  virtual ~Intake();

  /**
   * Engages the intake.
   * Runs the motor.
   */
  void engage(int16_t target_millivolts = INTAKE_POWER_LEVEL);

  void reverse(int16_t target_millivolts = INTAKE_POWER_LEVEL);

  /**
   * Disengages the intake.
   * Stops providing power to run the motor. Does not immediately stop.
   */
  void disengage();

  void roll_to_team_colour(config::AllianceColour teamColour, uint32_t timeout);

  /**
   * Returns whether the intake is currently engaged (running).
   * @return whether the intake is currently engaged (running).
   */
  [[nodiscard]] bool isEngaged() const;

  [[nodiscard]] const device::Motor &get_motor() const;

  void update(Controller *controller) override;
};
} // namespace robot
#endif // ROBOT_INTAKE_HPP
