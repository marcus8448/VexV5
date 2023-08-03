#include "control/input/operator.hpp"
#include "debug/error.hpp"
#include "debug/logger.hpp"

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

[[nodiscard]] int16_t Operator::speedSetting() const { return this->flywheelSpeed; }

void Operator::setSpeedSetting(int16_t speed) { this->flywheelSpeed = speed; }

void Operator::setLine(uint8_t line, uint8_t col, const char *str) {
  print_error("controller", pros::c::controller_set_text(this->controllerId, line, col, str));
}

void Operator::clearLine(uint8_t line) {
  print_error("controller", pros::c::controller_clear_line(this->controllerId, line));
}

void Operator::rumble(const char *str) {
  clear_error();
  pros::c::controller_rumble(this->controllerId, str);
  if (get_error() == EAGAIN) {
    debug("Failed to send rumble. Trying again soon.");
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

  if (this->rightPressed() % 2 == 1) {
    this->setSpeedSetting(static_cast<int16_t>(std::min(this->speedSetting() + 10, 600)));
  } else if (this->leftPressed() % 2 == 1) {
    this->setSpeedSetting(static_cast<int16_t>(std::max(this->speedSetting() - 10, 200)));
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

  if (enqueuedRumble != nullptr && this->ticks % 10 == 0) {
    clear_error();
    this->rumble(this->enqueuedRumble);
    if (get_error() != EAGAIN) {
      this->enqueuedRumble = nullptr;
    }
  }

  static bool init = false;
  if (this->ticks % 10 == 0 || !init) {
    init = true;
    this->setLine(0, 0,
                  logger::string_format("Flywheel: %i  ", static_cast<int32_t>(this->speedSetting()))
                      .c_str()); // append ' ' to clear out buffer
  }
}
} // namespace control::input
