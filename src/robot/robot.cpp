#include "robot/robot.hpp"
#include "control/autonomous/autonomous.hpp"
#include "debug/logger.hpp"

namespace robot {
Robot::Robot(uint8_t rightFront, uint8_t leftFront, uint8_t rightBack, uint8_t leftBack,
             uint8_t inertial, uint8_t claw)
    : drivetrain(rightFront, leftFront, rightBack, leftBack, inertial), claw(claw), controller(nullptr) {}

Robot::~Robot() {
  warn("Robot destructor called");
  delete controller;
  controller = nullptr;
}

void Robot::updateDevices() {
  this->drivetrain.updateState();
  this->claw.updateState();
}

[[noreturn]] void Robot::opcontrol() {
  this->drivetrain.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

  while (true) {
    this->updateDevices();
    if (this->controller != nullptr) {
      this->controller->update();
      this->drivetrain.updateTargeting(this->controller);
      this->claw.updateTargeting(this->controller);
    } else {
      error("Controller is null!");
    }
    pros::c::delay(ROBOT_TICK_RATE);
  }
}

void Robot::runAutonomous() {
  this->drivetrain.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  this->drivetrain.tare();

  pros::c::task_create([](void *param) {
    auto robot = static_cast<Robot *>(param);
    while (true) {
      robot->updateDevices();
      pros::c::delay(ROBOT_TICK_RATE);
    }
  }, this,TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Robot async control (OP)");

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
