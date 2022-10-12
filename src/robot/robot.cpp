#include "robot/robot.hpp"
#include "logger.hpp"

namespace robot {
Robot::Robot(Drivetrain *drivetrain, Intake *intake, Flywheel *flywheel)
    : drivetrain(drivetrain), intake(intake), flywheel(flywheel), controller(nullptr) {}

void Robot::update() {
  if (this->controller != nullptr) {
    this->controller->update();
    if (this->drivetrain != nullptr) {
      this->drivetrain->update(this->controller);
    } else {
      logger::error("Drivetrain is null?");
    }
    if (this->intake != nullptr) {
      this->intake->update(this->controller);
    } else {
      logger::error("Intake is null?");
    }
    if (this->flywheel != nullptr) {
      this->flywheel->update(this->controller);
    } else {
      logger::error("Flywheel is null?");
    }
  } else {
    logger::error("Controller is null!");
  }
}

void Robot::reset() { this->drivetrain->reset(); }

void Robot::stop() { this->drivetrain->stop(); }

Robot::~Robot() {
  logger::warn("Robot destructor called");
  delete controller;
  controller = nullptr;
  delete drivetrain;
  drivetrain = nullptr;
  delete intake;
  intake = nullptr;
  delete flywheel;
  flywheel = nullptr;
}
} // namespace robot