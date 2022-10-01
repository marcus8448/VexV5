#include "robot/controller/operator.hpp"
#include "logger.hpp"
#include "error.hpp"

namespace robot::controller {
OpController::OpController(pros::Controller controller) : controller(controller) {
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

  if (a == 1)
    logger::debug("A pressed");
  if (b == 1)
    logger::debug("B pressed");
  if (x == 1)
    logger::debug("X pressed");
  if (y == 1)
    logger::debug("Y pressed");
  if (up == 1)
    logger::debug("Up pressed");
  if (down == 1)
    logger::debug("Down pressed");
  if (left == 1)
    logger::debug("Left pressed");
  if (right == 1)
    logger::debug("Right pressed");
  if (r1 == 1)
    logger::debug("R1 pressed");
  if (r2 == 1)
    logger::debug("R2 pressed");
  if (l1 == 1)
    logger::debug("L1 pressed");
  if (l2 == 1)
    logger::debug("L2 pressed");

  if (this->up_pressed() || this->down_pressed()) {
    this->set_line(0, 0, ("Dig Spd: " + std::to_string(this->digital_speed()).append(
        " ")).c_str());//append ' ' to clear out buffer
  }
}
}
