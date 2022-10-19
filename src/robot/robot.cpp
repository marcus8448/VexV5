#include "robot/robot.hpp"
#include "logger.hpp"

namespace robot {
Robot::Robot(Drivetrain *drivetrain, Intake *intake, Indexer *indexer, Flywheel *flywheel)
    : drivetrain(drivetrain), intake(intake), indexer(indexer), flywheel(flywheel), controller(nullptr) {}

Robot::~Robot() {
  logger::warn("Robot destructor called");
  delete controller;
  controller = nullptr;
  delete drivetrain;
  drivetrain = nullptr;
  delete intake;
  intake = nullptr;
  delete indexer;
  indexer = nullptr;
  delete flywheel;
  flywheel = nullptr;
}

void Robot::update() const {
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
    if (this->indexer != nullptr) {
      this->indexer->update(this->controller);
    } else {
      logger::error("Indexer is null?");
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
} // namespace robot
