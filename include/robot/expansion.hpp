#ifndef ROBOT_EXPANSION_HPP
#define ROBOT_EXPANSION_HPP

#include "configuration.hpp"
#include "pros/motors.hpp"
#include "robot/controller/controller.hpp"

namespace robot {
/**
 * Represents the expansion on the robot.
 * Also controls the roller.
 */
class Expansion : public Updatable {
private:
  /**
   * The motor of the expansion/roller.
   */
  pros::Motor *motor;

  /**
   * Whether the mechanism has been released.
   */
  bool charged = false;

public:
  /**
   * Creates a new expansion with the specified motor.
   * @param motor the expansion's motor.
   */
  explicit Expansion(pros::Motor *motor);
  virtual ~Expansion();

  void launch();

  void charge();

  [[nodiscard]] bool has_launched() const;

  [[nodiscard]] pros::Motor *get_motor() const;

  void update(Controller *controller) override;
};
} // namespace robot
#endif // ROBOT_EXPANSION_HPP
