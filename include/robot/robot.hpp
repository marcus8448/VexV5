#ifndef ROBOT_ROBOT_HPP
#define ROBOT_ROBOT_HPP

#include "attributes.hpp"
#include "controller/controller.hpp"
#include "drivetrain.hpp"
#include "flywheel.hpp"

namespace robot {
class Robot : public Resettable {
public:
  Drivetrain *drivetrain;
  Flywheel *flywheel;
  Controller *controller;

public:
  explicit Robot(Drivetrain *drivetrain, Flywheel *flywheel);

  void update();
  void reset() override;
  void stop() override;
  ~Robot();
};
}

#endif//ROBOT_ROBOT_HPP
