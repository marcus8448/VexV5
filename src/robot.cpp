#include "robot.hpp"
#include "error.hpp"
#include "logger.hpp"
#include "pros/motors.h"
#include "util.hpp"
#include <cmath>

double DualPositioned::get_position() {
  return (this->get_left_position() + this->get_right_position()) / 2.0;
}

double DualPositioned::get_target_position() {
  return (this->get_left_target_position() + this->get_right_target_position()) / 2.0;
}

uint16_t OpController::a_pressed() {
  return this->a;
}

uint16_t OpController::b_pressed() {
  return this->b;
}

uint16_t OpController::x_pressed() {
  return this->x;
}

uint16_t OpController::y_pressed() {
  return this->y;
}

uint16_t OpController::up_pressed() {
  return this->up;
}

uint16_t OpController::down_pressed() {
  return this->down;
}

uint16_t OpController::left_pressed() {
  return this->left;
}

uint16_t OpController::right_pressed() {
  return this->right;
}

uint16_t OpController::l1_pressed() {
  return this->l1;
}

uint16_t OpController::l2_pressed() {
  return this->l2;
}

uint16_t OpController::r1_pressed() {
  return this->r1;
}

uint16_t OpController::r2_pressed() {
  return this->r2;
}

double OpController::left_stick_x() {
  return this->leftStickX;
}

double OpController::left_stick_y() {
  return this->leftStickY;
}

double OpController::right_stick_x() {
  return this->rightStickX;
}

double OpController::right_stick_y() {
  return this->rightStickY;
}

double OpController::prev_left_stick_x() {
  return this->prevLeftStickX;
}

double OpController::prev_left_stick_y() {
  return this->prevLeftStickY;
}

double OpController::prev_right_stick_x() {
  return this->prevRightStickX;
}

double OpController::prev_right_stick_y() {
  return this->prevRightStickY;
}

uint8_t OpController::digital_speed() {
  return this->digitalSpeed;
}

void OpController::digital_speed(uint8_t speed) {
  this->digitalSpeed = speed;
}

void OpController::set_line(uint8_t line, uint8_t col, const char *str) {
  check_error(this->controller.set_text(line, col, str));
}

void OpController::clear_line(uint8_t line) {
  check_error(this->controller.clear_line(line));
}

void OpController::rumble(const char *str) {
  check_error(this->controller.rumble(str));
}

void OpController::update() {
  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))) {
    this->a++;
  } else {
    this->a = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))) {
    this->b++;
  } else {
    this->b = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))) {
    this->x++;
  } else {
    this->x = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y))) {
    this->y++;
  } else {
    this->y = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP))) {
    this->up++;
  } else {
    this->up = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))) {
    this->down++;
  } else {
    this->down = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))) {
    this->left++;
  } else {
    this->left = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))) {
    this->right++;
  } else {
    this->right = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))) {
    this->l1++;
  } else {
    this->l1 = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))) {
    this->l2++;
  } else {
    this->l2 = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))) {
    this->r1++;
  } else {
    this->r1 = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))) {
    this->r2++;
  } else {
    this->r2 = 0;
  }

  this->prevLeftStickX = this->leftStickX;
  this->leftStickX = check_error(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X));

  this->prevLeftStickY = this->leftStickY;
  this->leftStickY = check_error(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));

  this->prevRightStickX = this->rightStickX;
  this->rightStickX = check_error(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));

  this->prevRightStickY = this->rightStickY;
  this->rightStickY = check_error(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));

  if (this->up_pressed()) {
    this->digital_speed(std::min(this->digital_speed() + 1, 127));
  } else if (this->down_pressed()) {
    this->digital_speed(std::max(this->digital_speed() - 1, 127));
  }

  if (a == 1) logger::debug("A pressed");
  if (b == 1) logger::debug("B pressed");
  if (x == 1) logger::debug("X pressed");
  if (y == 1) logger::debug("Y pressed");
  if (up == 1) logger::debug("Up pressed");
  if (down == 1) logger::debug("Down pressed");
  if (left == 1) logger::debug("Left pressed");
  if (right == 1) logger::debug("Right pressed");
  if (r1 == 1) logger::debug("R1 pressed");
  if (r2 == 1) logger::debug("R2 pressed");
  if (l1 == 1) logger::debug("L1 pressed");
  if (l2 == 1) logger::debug("L2 pressed");

  if (this->up_pressed() || this->down_pressed()) {
    this->set_line(0, 0, ("Dig Spd: " + std::to_string(this->digital_speed()).append(
        " ")).c_str());//append ' ' to clear out buffer
  }
}

void OpController::stop() {
}

void OpController::reset() {
  this->a = 0;
  this->b = 0;
  this->x = 0;
  this->y = 0;
  this->up = 0;
  this->down = 0;
  this->left = 0;
  this->right = 0;
  this->l1 = 0;
  this->l2 = 0;
  this->r1 = 0;
  this->r2 = 0;
  this->leftStickX = 0;
  this->leftStickY = 0;
  this->rightStickX = 0;
  this->rightStickY = 0;
  this->prevLeftStickX = 0;
  this->prevLeftStickY = 0;
  this->prevRightStickX = 0;
  this->prevRightStickY = 0;
}

OpController::OpController(pros::Controller controller) : controller(controller) {
}

Drivetrain::Drivetrain(pros::Motor *rightFront, pros::Motor *leftFront, pros::Motor *rightBack, pros::Motor *leftBack)
    : rightFront(rightFront), leftFront(leftFront), rightBack(rightBack), leftBack(leftBack) {
  this->rightFront->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  this->rightBack->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  this->leftFront->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  this->leftBack->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
}

Drivetrain::~Drivetrain() {
  // delete this->rightFront;
  // delete this->rightBack;
  // delete this->leftFront;
  // delete this->leftBack;
}

bool Drivetrain::is_offset_within(double distance) {
  return fabs(check_error(this->rightFront->get_position()) - check_error(this->rightFront->get_target_position())) <
      distance &&
      fabs(check_error(this->leftFront->get_position()) - check_error(this->leftFront->get_target_position())) <
          distance &&
      fabs(check_error(this->rightBack->get_position()) - check_error(this->rightBack->get_target_position())) <
          distance &&
      fabs(check_error(this->leftBack->get_position()) - check_error(this->leftBack->get_target_position())) <
          distance;
}

void Drivetrain::move_for(double right_distance, double left_distance, uint32_t max_rpm, bool block) {
  check_error(this->rightFront->move_relative(right_distance, max_rpm));
  check_error(this->rightBack->move_relative(right_distance, max_rpm));
  check_error(this->leftFront->move_relative(left_distance, max_rpm));
  check_error(this->leftBack->move_relative(left_distance, max_rpm));
  if (block) {
    while (!this->is_offset_within(5.0)) {
      pros::delay(50);
    }
    do {
      pros::delay(25);
    } while (!this->is_offset_within(1.0));
  }
}

void Drivetrain::forwards(double distance, uint32_t max_rpm, bool block) {
  this->move_for(in_to_rot(distance), in_to_rot(distance), max_rpm, block);
}

void Drivetrain::backwards(double distance, uint32_t max_rpm, bool block) {
  this->move_for(-in_to_rot(distance), -in_to_rot(distance), max_rpm, block);
}

void Drivetrain::turn_right(double angle, uint32_t max_rpm, bool block) {
  this->move_for(-turn_to_rot(angle), turn_to_rot(angle), max_rpm, block);
}

void Drivetrain::turn_left(double angle, uint32_t max_rpm, bool block) {
  this->move_for(turn_to_rot(angle), -turn_to_rot(angle), max_rpm, block);
}

void Drivetrain::move_right(uint32_t voltage) {
  check_error(this->rightFront->move(voltage));
  check_error(this->rightBack->move(voltage));
}

void Drivetrain::move_left(uint32_t voltage) {
  check_error(this->leftFront->move(voltage));
  check_error(this->leftBack->move(voltage));
}

void Drivetrain::update(Controller *controller) {
  this->move_right((int) controller->right_stick_y());
  this->move_left((int) controller->left_stick_y());
}

void Drivetrain::stop() {
  this->rightFront->move(0);
  this->leftFront->move(0);
  this->rightBack->move(0);
  this->leftBack->move(0);
}

void Drivetrain::reset() {
  this->rightFront->tare_position();
  this->leftFront->tare_position();
  this->rightBack->tare_position();
  this->leftBack->tare_position();
}

Robot::Robot(Drivetrain *drivetrain, Flywheel *flywheel) : drivetrain(drivetrain), flywheel(flywheel), controller(nullptr) {
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
  this->controller->reset();
  this->drivetrain->reset();
}

void Robot::stop() {
  this->controller->stop();
  this->drivetrain->stop();
}

Robot::~Robot() {
  logger::info("Robot destructor called");
  delete controller;
  controller = nullptr;
  delete drivetrain;
  drivetrain = nullptr;
}

Flywheel::Flywheel(pros::Motor *motor): motor(motor) {
  motor->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

void Flywheel::engage() {
  this->motor->move(-127);
  this->engaged = true;
}

void Flywheel::disengage() {
  this->engaged = false;
  this->motor->move(0);
}

bool Flywheel::isEngaged() {
  return this->engaged;
}

void Flywheel::update(Controller *controller) {
  if (controller->r1_pressed()) {
    this->engage();
  } else if (controller->r2_pressed()) {
    this->disengage();
  }
}
