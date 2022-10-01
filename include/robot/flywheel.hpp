#ifndef ROBOT_FLYWHEEL_HPP
#define ROBOT_FLYWHEEL_HPP

#include "attributes.hpp"
#include "pros/motors.hpp"
#include "robot/controller/controller.hpp"

namespace robot {
class Flywheel : public Updatable {
public:
  pros::Motor *motor;

private:
  bool engaged = false;

public:
  explicit Flywheel(pros::Motor *motor);

  void engage();
  void disengage();

  [[nodiscard]] bool isEngaged() const;
  void update(Controller *controller) override;
};
} // namespace robot
#endif // ROBOT_FLYWHEEL_HPP
