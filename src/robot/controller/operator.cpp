#include "robot/controller/operator.hpp"
#include "error.hpp"
#include "logger.hpp"

namespace robot::controller {
OpController::OpController(pros::Controller controller) : controller(controller) {}

[[nodiscard]] uint16_t OpController::a_pressed() const { return this->a; }

[[nodiscard]] uint16_t OpController::b_pressed() const { return this->b; }

[[nodiscard]] uint16_t OpController::x_pressed() const { return this->x; }

[[nodiscard]] uint16_t OpController::y_pressed() const { return this->y; }

[[nodiscard]] uint16_t OpController::up_pressed() const { return this->up; }

[[nodiscard]] uint16_t OpController::down_pressed() const { return this->down; }

[[nodiscard]] uint16_t OpController::left_pressed() const { return this->left; }

[[nodiscard]] uint16_t OpController::right_pressed() const { return this->right; }

[[nodiscard]] uint16_t OpController::l1_pressed() const { return this->l1; }

[[nodiscard]] uint16_t OpController::l2_pressed() const { return this->l2; }

[[nodiscard]] uint16_t OpController::r1_pressed() const { return this->r1; }

[[nodiscard]] uint16_t OpController::r2_pressed() const { return this->r2; }

[[nodiscard]] double OpController::left_stick_x() const { return this->leftStickX; }

[[nodiscard]] double OpController::left_stick_y() const { return this->leftStickY; }

[[nodiscard]] double OpController::right_stick_x() const { return this->rightStickX; }

[[nodiscard]] double OpController::right_stick_y() const { return this->rightStickY; }

[[nodiscard]] double OpController::prev_left_stick_x() const { return this->prevLeftStickX; }

[[nodiscard]] double OpController::prev_left_stick_y() const { return this->prevLeftStickY; }

[[nodiscard]] double OpController::prev_right_stick_x() const { return this->prevRightStickX; }

[[nodiscard]] double OpController::prev_right_stick_y() const { return this->prevRightStickY; }

[[nodiscard]] int16_t OpController::flywheel_speed() const { return this->flywheelSpeed; }

void OpController::flywheel_speed(int16_t speed) { this->flywheelSpeed = speed; }

void OpController::set_line(uint8_t line, uint8_t col, const char *str) {
  print_error(this->controller.set_text(line, col, str));
}

void OpController::clear_line(uint8_t line) { print_error(this->controller.clear_line(line)); }

void OpController::rumble(char *str) {
  clear_error();
  this->controller.rumble(str);
  if (get_error() == EAGAIN) {
    logger::debug("Failed to send rumble. Trying again soon.");
    this->enqueued_rumble = str;
  }
}

void OpController::update() {
  this->ticks++;
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

  if (this->right_pressed()) {
    this->flywheel_speed(static_cast<int16_t>(std::min(this->flywheel_speed() + 100, 12000)));
  } else if (this->left_pressed()) {
    this->flywheel_speed(static_cast<int16_t>(std::max(this->flywheel_speed() - 100, 4000)));
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

  if (enqueued_rumble != nullptr) {
    clear_error();
    this->rumble(this->enqueued_rumble);
    if (get_error() != EAGAIN) {
      this->enqueued_rumble = nullptr;
    } 
  }

  static bool init = false;
  if (this->ticks % 10 == 0 || !init) {
    init = true;
    this->set_line(0, 0,
                   logger::string_format("Flywheel: %i  ", static_cast<int32_t>(this->flywheel_speed()))
                       .c_str()); // append ' ' to clear out buffer
  }
}
} // namespace robot::controller
