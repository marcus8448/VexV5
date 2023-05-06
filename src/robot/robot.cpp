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

[[noreturn]] void Robot::background_control() {
  while (true) {
    this->drivetrain.updatePosition();

    this->drivetrain.updateMovement();
    pros::delay(ROBOT_TICK_RATE);
  }
}

[[noreturn]] void Robot::opcontrol() {
  this->drivetrain.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

  pros::c::task_create([](void *param) { static_cast<Robot *>(param)->background_control(); }, this,
                       TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Robot async control (OP)");

  while (true) {
    if (this->controller != nullptr) {
      this->controller->update();
      this->drivetrain.updateTargeting(this->controller);
    } else {
      error("Controller is null!");
    }
    pros::delay(ROBOT_TICK_RATE);
  }
}

void Robot::run_autonomous() {
  this->drivetrain.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  pros::c::task_create([](void *param) { static_cast<Robot *>(param)->background_control(); }, this,
                       TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Robot async control (OP)");

  if (autonomous::get_active() == nullptr) {
    error("No autonomous to run!");
    return;
  }
  info("Running autonomous: '%s'", autonomous::get_active()->c_str());
  autonomous::get_autonomous()->run(*this);
}

void Robot::set_controller(Controller *controller) {
  delete this->controller;
  this->controller = controller;
}
} // namespace robot
