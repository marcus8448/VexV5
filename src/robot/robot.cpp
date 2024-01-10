#include "robot.hpp"
#include "control/autonomous/autonomous.hpp"
#include "debug/logger.hpp"
#include "pros/rtos.h"
#include "tasks.hpp"

namespace robot {
Robot::Robot(int8_t driveL1, int8_t driveL2, int8_t driveL3, int8_t driveR1, int8_t driveR2, int8_t driveR3,
             int8_t wingsL, int8_t wingsR, int8_t inertial, int8_t catapult, int8_t catapult2, int8_t catapultR)
    : drivetrain(driveL1, driveL2, driveL3, driveR1, driveR2, driveR3, inertial), wings(wingsL, wingsR),
      catapult(catapult, catapult2, catapultR), controller(nullptr) {}

Robot::~Robot() { logger::error("Robot destructor called"); }

void Robot::updateDevices() {
  this->drivetrain.updateState();
  this->catapult.updateState();
}

[[noreturn]] void Robot::opcontrol() {
  this->drivetrain.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

  while (true) {
    this->updateDevices();

    if (this->controller != nullptr) {
      this->controller->update();
      this->drivetrain.updateTargeting(this->controller.get());
      this->wings.updateTargeting(this->controller.get());
      this->catapult.updateTargeting(this->controller.get());
    } else {
      logger::error("Controller is null!");
    }

    pros::c::delay(device::TICK_RATE);
  }
}

[[noreturn]] void autonomousBackground(void *param) {
  auto &robot = *static_cast<Robot *>(param);
  while (true) {
    robot.updateDevices();
    pros::c::delay(device::TICK_RATE);
  }
}

void Robot::runAutonomous() {
  if (control::autonomous::getPrograms().contains(this->autonomous)) {
    this->drivetrain.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    this->drivetrain.tare();

    const pros::task_t task = rtos::createChildTask("Robot async control", autonomousBackground, this);

    logger::info("Running autonomous: '{}'", this->autonomous.c_str());
    control::autonomous::getPrograms()[this->autonomous](*this);
    rtos::killTask(task);
  } else {
    logger::error("No autonomous to run!");
  }
}

void Robot::setController(control::input::Controller *controller) { this->controller.reset(controller); }
} // namespace robot
