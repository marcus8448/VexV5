#include "robot.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "pros/motors.h"
#include "util.hpp"
#include "drivetrain.hpp"
#include "flywheel.hpp"
#include <cmath>

Robot::Robot(Drivetrain *drivetrain, Flywheel *flywheel)
    : drivetrain(drivetrain), flywheel(flywheel), controller(nullptr) {
}

void Robot::update() {
  if (this->controller == nullptr) {
    logger::info("Controller is null?");
  }
  if (this->drivetrain == nullptr) {
    logger::info("Drivetrain is null?");
  }
  if (this->flywheel == nullptr) {
    logger::info("Flywheel is null?");
  }
  this->controller->update();
  this->drivetrain->update(this->controller);
  this->flywheel->update(this->controller);
}

void Robot::reset() {
  this->drivetrain->reset();
}

void Robot::stop() {
  this->drivetrain->stop();
}

Robot::~Robot() {
  logger::info("Robot destructor called");
  delete controller;
  controller = nullptr;
  delete drivetrain;
  drivetrain = nullptr;
}
