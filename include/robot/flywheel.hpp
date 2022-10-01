#ifndef ROBOT_FLYWHEEL_HPP
#define ROBOT_FLYWHEEL_HPP

#include "pros/motors.hpp"
#include "attributes.hpp"
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

  bool isEngaged();
  void update(Controller *controller) override;
};
}
#endif //ROBOT_FLYWHEEL_HPP
