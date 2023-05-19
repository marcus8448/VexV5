#include "robot/robot.hpp"
#include "control/autonomous/autonomous.hpp"
#include "debug/logger.hpp"

namespace robot {
Robot::Robot(const uint8_t rightFront, const uint8_t leftFront, const uint8_t rightBack, const uint8_t leftBack,
             const uint8_t inertial)
    : drivetrain(rightFront, leftFront, rightBack, leftBack, inertial), controller(nullptr) {}

Robot::~Robot() {
  warn("Robot destructor called");
  delete controller;
  controller = nullptr;
}

[[noreturn]] void Robot::backgroundControl() {
  while (true) {
    this->drivetrain.updatePosition();

    this->drivetrain.updateMovement();
    pros::c::delay(ROBOT_TICK_RATE);
  }
}

[[noreturn]] void Robot::opcontrol() {
  this->drivetrain.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

  pros::c::task_create([](void *param) { static_cast<Robot *>(param)->backgroundControl(); }, this,
                       TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Robot async control (OP)");

  while (true) {
    if (this->controller != nullptr) {
      this->controller->update();
      this->drivetrain.updateTargeting(this->controller);
    } else {
      error("Controller is null!");
    }
    pros::c::delay(ROBOT_TICK_RATE);
  }
}

void Robot::runAutonomous() {
  this->drivetrain.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  this->drivetrain.tare();

  pros::c::task_create([](void *param) { static_cast<Robot *>(param)->backgroundControl(); }, this,
                       TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Robot async control (OP)");

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
