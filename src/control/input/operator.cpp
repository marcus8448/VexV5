#include "control/input/operator.hpp"
#include "debug/error.hpp"
#include "debug/logger.hpp"
#include "format.hpp"

#define controller_digital(button)                                                                                     \
  print_error("controller", pros::c::controller_get_digital(this->controllerId, button))
#define controller_analog(stick_axis)                                                                                  \
  print_error("controller", pros::c::controller_get_analog(this->controllerId, stick_axis))

namespace control::input {
Operator::Operator(pros::controller_id_e_t controller_id) : controllerId(controller_id) {}

[[nodiscard]] uint16_t Operator::aPressed() const { return this->a; }

[[nodiscard]] uint16_t Operator::bPressed() const { return this->b; }

[[nodiscard]] uint16_t Operator::xPressed() const { return this->x; }

[[nodiscard]] uint16_t Operator::yPressed() const { return this->y; }

[[nodiscard]] uint16_t Operator::upPressed() const { return this->up; }

[[nodiscard]] uint16_t Operator::downPressed() const { return this->down; }

[[nodiscard]] uint16_t Operator::leftPressed() const { return this->left; }

[[nodiscard]] uint16_t Operator::rightPressed() const { return this->right; }

[[nodiscard]] uint16_t Operator::l1Pressed() const { return this->l1; }

[[nodiscard]] uint16_t Operator::l2Pressed() const { return this->l2; }

[[nodiscard]] uint16_t Operator::r1Pressed() const { return this->r1; }

[[nodiscard]] uint16_t Operator::r2Pressed() const { return this->r2; }

[[nodiscard]] double Operator::leftStickX() const { return this->lsX; }

[[nodiscard]] double Operator::leftStickY() const { return this->lsY; }

[[nodiscard]] double Operator::rightStickX() const { return this->rsX; }

[[nodiscard]] double Operator::rightStickY() const { return this->rsY; }

void Operator::setLine(uint8_t line, const char *str) {
  text[line] = str;
  textDirty[line] = true;
}

void Operator::clearLine(uint8_t line) {
  text[line] = nullptr;
  textDirty[line] = true;
}

void Operator::rumble(const char *str) {
  clear_error();
  pros::c::controller_rumble(this->controllerId, str);
  if (get_error() == EAGAIN) {
    logger::debug("Failed to send rumble. Trying again soon.");
    this->enqueuedRumble = str;
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

  this->lsX = controller_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
  this->lsY = controller_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
  this->rsX = controller_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
  this->rsY = controller_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

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

  if (enqueuedRumble != nullptr && this->ticks % 10 == 0) {
    clear_error();
    this->rumble(this->enqueuedRumble);
    if (get_error() != EAGAIN) {
      this->enqueuedRumble = nullptr;
    }
  }

  for (auto i = 0; i < LINE_COUNT; ++i) {
    if (this->textDirty[i]) {
      bool success = false;
      if (this->text[i] != nullptr) {
        success = pros::c::controller_set_text(this->controllerId, i, 0, this->text[i]) == 1;
      } else {
        success = pros::c::controller_clear_line(this->controllerId, i) == 1;
      }
      errno = 0;
      if (success) {
        this->textDirty[i] = false;
        break; // only one success per cycle
      }
    }
  }
}
} // namespace control::input
