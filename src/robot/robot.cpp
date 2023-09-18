#include "robot/robot.hpp"
#include "control/autonomous/autonomous.hpp"
#include "debug/logger.hpp"
#include "pros/rtos.h"
#include "tasks.hpp"

#ifdef ENABLE_TEMPORARY_CODE
#include "../temporary.cpp"
#endif

namespace robot {
Robot::Robot(int8_t driveL1, int8_t driveL2, int8_t driveL3, int8_t driveR1, int8_t driveR2, int8_t driveR3,
             int8_t intakeRight, int8_t intakeLeft, int8_t inertial, int8_t arm1, int8_t arm2)
    : drivetrain(driveL1, driveL2, driveL3, driveR1, driveR2, driveR3, inertial), arm(arm1, arm2),
      intake(intakeRight, intakeLeft), controller(nullptr) {}

Robot::~Robot() { logger::error("Robot destructor called"); }

void Robot::updateDevices() {
  this->drivetrain.updateState();
  this->intake.updateState();
}

[[noreturn]] void Robot::opcontrol() {
  this->drivetrain.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

  while (true) {
    this->updateDevices();

    if (this->controller != nullptr) {
      this->controller->update();
      this->drivetrain.updateTargeting(this->controller.get());
      this->arm.updateTargeting(this->controller.get());
      this->intake.updateTargeting(this->controller.get());
    } else {
      logger::error("Controller is null!");
    }

    pros::c::delay(robot::device::TICK_RATE);
  }
}

[[noreturn]] void autonomousBackground(void *param) {
  auto &robot = *static_cast<Robot *>(param);
  while (true) {
    robot.updateDevices();
    pros::c::delay(robot::device::TICK_RATE);
  }
}
#ifdef ENABLE_TEMPORARY_CODE
void Robot::runAutonomous() {
  logger::warn("overriding autonomous");
  this->drivetrain.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
  this->drivetrain.tare();

  pros::task_t task = rtos::createChildTask("Robot async control", autonomousBackground, this);
  temporary::run(*this);
  rtos::killTask(task);
}
#endif

void Robot::runAutonomous() {
  if (control::autonomous::getPrograms().contains(this->autonomous)) {
    this->drivetrain.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    this->drivetrain.tare();

    pros::task_t task = rtos::createChildTask("Robot async control", autonomousBackground, this);

    logger::info("Running autonomous: '%s'", this->autonomous.c_str());
    control::autonomous::getPrograms()[this->autonomous](*this);
    rtos::killTask(task);
  } else {
    logger::error("No autonomous to run!");
  }
}

void Robot::setController(control::input::Controller *controller) { this->controller.reset(controller); }
} // namespace robot
