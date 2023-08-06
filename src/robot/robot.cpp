#include "robot/robot.hpp"
#include "control/autonomous/autonomous.hpp"
#include "debug/logger.hpp"
#include "tasks.hpp"
#include "pros/rtos.h"

#ifdef ENABLE_TEMPORARY_CODE
#include "../temporary.cpp"
#endif

namespace robot {
Robot::Robot(uint8_t driveLeftFront, uint8_t driveRightFront, uint8_t driveLeftBack, uint8_t driveRightBack,
             uint8_t intakeRight, uint8_t intakeLeft, uint8_t inertial, uint8_t arm1, uint8_t arm2)
    : drivetrain(driveLeftFront, driveRightFront, driveLeftBack, driveRightBack, inertial), arm(arm1, arm2),
      intake(intakeRight, intakeLeft), controller(nullptr) {}

Robot::~Robot() {
  warn("Robot destructor called");
  delete controller;
  controller = nullptr;
}

void Robot::updateDevices() {
  this->drivetrain.updateState();
  this->arm.updateState();
  this->intake.updateState();
}

[[noreturn]] void Robot::opcontrol() {
  this->drivetrain.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

  while (true) {
    this->updateDevices();

    if (this->controller != nullptr) {
      this->controller->update();
      this->drivetrain.updateTargeting(this->controller);
      this->arm.updateTargeting(this->controller);
      this->intake.updateTargeting(this->controller);
    } else {
      error("Controller is null!");
    }

    pros::c::delay(ROBOT_TICK_RATE);
  }
}

[[noreturn]] void autonomousBackground(void *param) {
  auto robot = static_cast<Robot *>(param);
  while (true) {
    robot->updateDevices();
    pros::c::delay(ROBOT_TICK_RATE);
  }
}

void Robot::runAutonomous() {
  this->drivetrain.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
  this->drivetrain.tare();

  createChildTask("Robot async control", autonomousBackground, this);

#ifdef ENABLE_TEMPORARY_CODE
  temporary::run(*this);
  return;
#endif

  if (control::autonomous::getActive() == nullptr) {
    error("No autonomous to run!");
    return;
  }
  info("Running autonomous: '%s'", control::autonomous::getActive()->c_str());
  control::autonomous::run(*this);
}

void Robot::setController(control::input::Controller *controller) {
  delete this->controller;
  this->controller = controller;
}
} // namespace robot
