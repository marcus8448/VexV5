#ifndef ROBOT_INTAKE_HPP
#define ROBOT_INTAKE_HPP

#define INTAKE_POWER_LEVEL 11000

#include "configuration.hpp"
#include "pros/optical.hpp"
#include "robot/controller/controller.hpp"
#include "robot/device/motor.hpp"
#include "robot/device/optical.hpp"
#include <vector>

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

  /**
   * Whether the intake is currently running.
   */
  int16_t speed = 0;
  bool reversed = false;

public:
  /**
   * Creates a new intake with the specified motor.
   * @param motor the intake's motor.
   */
  explicit Intake(uint8_t motorPort);
  virtual ~Intake();

  /**
   * Engages the intake.
   * Runs the motor.
   */
  void engage(int16_t mV = INTAKE_POWER_LEVEL);

  void reverse(int16_t mV = INTAKE_POWER_LEVEL);

  /**
   * Disengages the intake.
   * Stops providing power to run the motor. Does not immediately stop.
   */
  void disengage();

  /**
   * Returns whether the intake is currently engaged (running).
   * @return whether the intake is currently engaged (running).
   */
  [[nodiscard]] bool is_engaged() const;

  [[nodiscard]] const device::Motor &get_motor() const;

  void update(Controller *controller) override;
  double bring_roller_to_speed(int16_t mV);
};
} // namespace robot
#endif // ROBOT_INTAKE_HPP
