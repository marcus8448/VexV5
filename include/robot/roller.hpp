#ifndef ROBOT_ROLLER_HPP
#define ROBOT_ROLLER_HPP

#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "robot/controller/controller.hpp"
#include "configuration.hpp"

namespace robot {

/**
 * Represents the roller on the robot.
 */
class Roller : public Updatable {
private:
  /**
   * The motor of the roller.
   */
  pros::Motor *motor;
  pros::Optical *optical;

  /**
   * Whether the roller is currently running.
   */
  bool engaged = false;

public:
  /**
   * Creates a new roller with the specified motor and sensor.
   * @param motor the roller's motor.
   * @param optical the roller's optical sensor.
   */
  explicit Roller(pros::Motor *motor, pros::Optical *optical);
  virtual ~Roller();

  bool looking_at_roller();

  void spin_until_colour(config::AllianceColour teamColour, uint32_t timeout);

  [[nodiscard]] pros::Motor *get_motor() const;

  void update(Controller *controller) override;
};
} // namespace robot
#endif // ROBOT_ROLLER_HPP
