#ifndef ROBOT_EXPANSION_HPP
#define ROBOT_EXPANSION_HPP

#include "configuration.hpp"
#include "robot/controller/controller.hpp"
#include "robot/device/motor.hpp"

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
  device::Motor motor;

  /**
   * Whether the mechanism has been released.
   */
  bool charged = false;

public:
  /**
   * Creates a new expansion with the specified motor.
   * @param motor the expansion's motor.
   */
  explicit Expansion(uint8_t port);
  virtual ~Expansion();

  void launch();

  void charge();

  [[nodiscard]] bool has_launched() const;

  [[nodiscard]] const device::Motor &get_motor() const;

  void update(Controller *controller) override;
};
} // namespace robot
#endif // ROBOT_EXPANSION_HPP
