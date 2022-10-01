#include "robot/robot.hpp"
#include "logger.hpp"

namespace robot {
Robot::Robot(Drivetrain *drivetrain, Flywheel *flywheel)
    : drivetrain(drivetrain), flywheel(flywheel), controller(nullptr) {}

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

void Robot::reset() { this->drivetrain->reset(); }

void Robot::stop() { this->drivetrain->stop(); }

Robot::~Robot() {
  logger::warn("Robot destructor called");
  delete controller;
  controller = nullptr;
  delete drivetrain;
  drivetrain = nullptr;
}
} // namespace robot