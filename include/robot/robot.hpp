#ifndef ROBOT_ROBOT_HPP
#define ROBOT_ROBOT_HPP

#include "attributes.hpp"
#include "controller/controller.hpp"
#include "drivetrain.hpp"
#include "flywheel.hpp"
#include "intake.hpp"

namespace robot {
class Robot : public Resettable {
public:
  Drivetrain *drivetrain;
  Intake *intake;
  Flywheel *flywheel;
  Controller *controller;

public:
  explicit Robot(Drivetrain *drivetrain, Intake *intake, Flywheel *flywheel);

  void update();
  void reset() override;
  void stop() override;
  ~Robot();
};
} // namespace robot

#endif // ROBOT_ROBOT_HPP
