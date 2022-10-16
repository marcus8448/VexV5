#include "robot/controller/operator.hpp"
#include "error.hpp"
#include "logger.hpp"

namespace robot::controller {
OpController::OpController(pros::Controller controller) : controller(controller) {}

uint16_t OpController::a_pressed() const { return this->a; }

uint16_t OpController::b_pressed() const { return this->b; }

uint16_t OpController::x_pressed() const { return this->x; }

uint16_t OpController::y_pressed() const { return this->y; }

uint16_t OpController::up_pressed() const { return this->up; }

uint16_t OpController::down_pressed() const { return this->down; }

uint16_t OpController::left_pressed() const { return this->left; }

uint16_t OpController::right_pressed() const { return this->right; }

uint16_t OpController::l1_pressed() const { return this->l1; }

uint16_t OpController::l2_pressed() const { return this->l2; }

uint16_t OpController::r1_pressed() const { return this->r1; }

uint16_t OpController::r2_pressed() const { return this->r2; }

double OpController::left_stick_x() const { return this->leftStickX; }

double OpController::left_stick_y() const { return this->leftStickY; }

double OpController::right_stick_x() const { return this->rightStickX; }

double OpController::right_stick_y() const { return this->rightStickY; }

double OpController::prev_left_stick_x() const { return this->prevLeftStickX; }

double OpController::prev_left_stick_y() const { return this->prevLeftStickY; }

double OpController::prev_right_stick_x() const { return this->prevRightStickX; }

double OpController::prev_right_stick_y() const { return this->prevRightStickY; }

uint8_t OpController::digital_speed() const { return this->digitalSpeed; }

void OpController::digital_speed(uint8_t speed) { this->digitalSpeed = speed; }

void OpController::set_line(uint8_t line, uint8_t col, const char *str) {
  print_error(this->controller.set_text(line, col, str));
}

void OpController::clear_line(uint8_t line) { print_error(this->controller.clear_line(line)); }

void OpController::rumble(const char *str) { print_error(this->controller.rumble(str)); }

void OpController::update() {
  if (print_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))) {
    this->a++;
  } else {
    this->a = 0;
  }

  if (print_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))) {
    this->b++;
  } else {
    this->b = 0;
  }

  if (print_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))) {
    this->x++;
  } else {
    this->x = 0;
  }

  if (print_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y))) {
    this->y++;
  } else {
    this->y = 0;
  }

  if (print_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP))) {
    this->up++;
  } else {
    this->up = 0;
  }

  if (print_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))) {
    this->down++;
  } else {
    this->down = 0;
  }

  if (print_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))) {
    this->left++;
  } else {
    this->left = 0;
  }

  if (print_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))) {
    this->right++;
  } else {
    this->right = 0;
  }

  if (print_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))) {
    this->l1++;
  } else {
    this->l1 = 0;
  }

  if (print_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))) {
    this->l2++;
  } else {
    this->l2 = 0;
  }

  if (print_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))) {
    this->r1++;
  } else {
    this->r1 = 0;
  }

  if (print_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))) {
    this->r2++;
  } else {
    this->r2 = 0;
  }

  this->prevLeftStickX = this->leftStickX;
  this->leftStickX = print_error(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X));

  this->prevLeftStickY = this->leftStickY;
  this->leftStickY = print_error(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));

  this->prevRightStickX = this->rightStickX;
  this->rightStickX = print_error(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));

  this->prevRightStickY = this->rightStickY;
  this->rightStickY = print_error(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));

  if (this->up_pressed()) {
    this->digital_speed(std::min(this->digital_speed() + 1, 127));
  } else if (this->down_pressed()) {
    this->digital_speed(std::max(this->digital_speed() - 1, 127));
  }

  if (this->a == 1)
    logger::debug("A pressed");
  if (this->b == 1)
    logger::debug("B pressed");
  if (this->x == 1)
    logger::debug("X pressed");
  if (this->y == 1)
    logger::debug("Y pressed");
  if (this->up == 1)
    logger::debug("Up pressed");
  if (this->down == 1)
    logger::debug("Down pressed");
  if (this->left == 1)
    logger::debug("Left pressed");
  if (this->right == 1)
    logger::debug("Right pressed");
  if (this->r1 == 1)
    logger::debug("R1 pressed");
  if (this->r2 == 1)
    logger::debug("R2 pressed");
  if (this->l1 == 1)
    logger::debug("L1 pressed");
  if (this->l2 == 1)
    logger::debug("L2 pressed");

  if (this->up_pressed() || this->down_pressed()) {
    this->set_line(
        0, 0, logger::string_format("Dig Spd: %i  ", this->digital_speed()).c_str()); // append ' ' to clear out buffer
  }
}
} // namespace robot::controller
