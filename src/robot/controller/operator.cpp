#include "robot/controller/operator.hpp"
#include "error.hpp"
#include "logger.hpp"

#define controller_digital(button) print_error(pros::c::controller_get_digital(this->controller_id, button))
#define controller_analog(stick_axis) print_error(pros::c::controller_get_analog(this->controller_id, stick_axis))

namespace robot::controller {
Operator::Operator(pros::controller_id_e_t controller_id) : controller_id(controller_id) {}

[[nodiscard]] uint16_t Operator::a_pressed() const { return this->a; }

[[nodiscard]] uint16_t Operator::b_pressed() const { return this->b; }

[[nodiscard]] uint16_t Operator::x_pressed() const { return this->x; }

[[nodiscard]] uint16_t Operator::y_pressed() const { return this->y; }

[[nodiscard]] uint16_t Operator::up_pressed() const { return this->up; }

[[nodiscard]] uint16_t Operator::down_pressed() const { return this->down; }

[[nodiscard]] uint16_t Operator::left_pressed() const { return this->left; }

[[nodiscard]] uint16_t Operator::right_pressed() const { return this->right; }

[[nodiscard]] uint16_t Operator::l1_pressed() const { return this->l1; }

[[nodiscard]] uint16_t Operator::l2_pressed() const { return this->l2; }

[[nodiscard]] uint16_t Operator::r1_pressed() const { return this->r1; }

[[nodiscard]] uint16_t Operator::r2_pressed() const { return this->r2; }

[[nodiscard]] double Operator::left_stick_x() const { return this->leftStickX; }

[[nodiscard]] double Operator::left_stick_y() const { return this->leftStickY; }

[[nodiscard]] double Operator::right_stick_x() const { return this->rightStickX; }

[[nodiscard]] double Operator::right_stick_y() const { return this->rightStickY; }

[[nodiscard]] int16_t Operator::flywheel_speed() const { return this->flywheelSpeed; }

void Operator::flywheel_speed(int16_t speed) { this->flywheelSpeed = speed; }

void Operator::set_line(uint8_t line, uint8_t col, const char *str) {
  print_error(pros::c::controller_set_text(this->controller_id, line, col, str));
}

void Operator::clear_line(uint8_t line) { print_error(pros::c::controller_clear_line(this->controller_id, line)); }

void Operator::rumble(const char *str) {
  clear_error();
  pros::c::controller_rumble(this->controller_id, str);
  if (get_error() == EAGAIN) {
    debug("Failed to send rumble. Trying again soon.");
    this->enqueued_rumble = str;
  }
}

void Operator::update() {
  this->ticks++;
  if (controller_digital(pros::E_CONTROLLER_DIGITAL_A)) {
    this->a++;
  } else {
    this->a = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_B)) {
    this->b++;
  } else {
    this->b = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_X)) {
    this->x++;
  } else {
    this->x = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
    this->y++;
  } else {
    this->y = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
    this->up++;
  } else {
    this->up = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
    this->down++;
  } else {
    this->down = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
    this->left++;
  } else {
    this->left = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
    this->right++;
  } else {
    this->right = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
    this->l1++;
  } else {
    this->l1 = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
    this->l2++;
  } else {
    this->l2 = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
    this->r1++;
  } else {
    this->r1 = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
    this->r2++;
  } else {
    this->r2 = 0;
  }

  this->leftStickX = controller_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
  this->leftStickY = controller_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
  this->rightStickX = controller_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
  this->rightStickY = controller_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

  if (this->right_pressed() % 2 == 1) {
    this->flywheel_speed(static_cast<int16_t>(std::min(this->flywheel_speed() + 10, 600)));
  } else if (this->left_pressed() % 2 == 1) {
    this->flywheel_speed(static_cast<int16_t>(std::max(this->flywheel_speed() - 10, 200)));
  }

  if (this->a == 1)
    debug("A pressed");
  if (this->b == 1)
    debug("B pressed");
  if (this->x == 1)
    debug("X pressed");
  if (this->y == 1)
    debug("Y pressed");
  if (this->up == 1)
    debug("Up pressed");
  if (this->down == 1)
    debug("Down pressed");
  if (this->left == 1)
    debug("Left pressed");
  if (this->right == 1)
    debug("Right pressed");
  if (this->r1 == 1)
    debug("R1 pressed");
  if (this->r2 == 1)
    debug("R2 pressed");
  if (this->l1 == 1)
    debug("L1 pressed");
  if (this->l2 == 1)
    debug("L2 pressed");

  if (enqueued_rumble != nullptr && this->ticks % 10 == 0) {
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
