#ifndef ROBOT_INTAKE_HPP
#define ROBOT_INTAKE_HPP

#include "pros/motors.hpp"
#include "robot/attributes.hpp"
#include "robot/controller/controller.hpp"

namespace robot {
class Intake : public Updatable {
private:
  pros::Motor *motor;
  bool engaged = false;

public:
  explicit Intake(pros::Motor *motor);
  virtual ~Intake();

  void engage();
  void disengage();
  [[nodiscard]] bool isEngaged() const;

  void update(Controller *controller) override;
};
} // namespace robot
#endif // ROBOT_INTAKE_HPP