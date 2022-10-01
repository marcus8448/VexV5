#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "attributes.hpp"
#include "controller.hpp"
#include "drivetrain.hpp"
#include "flywheel.hpp"

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

#endif//ROBOT_HPP
