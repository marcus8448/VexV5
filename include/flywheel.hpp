#ifndef FLYWHEEL_HPP
#define FLYWHEEL_HPP

#include "pros/motors.hpp"
#include "attributes.hpp"
#include "controller.hpp"

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
#endif //FLYWHEEL_HPP
