#include "control/input/controller.hpp"
#include "debug/error.hpp"
#include "debug/logger.hpp"

namespace control::input {
Controller::Controller(pros::controller_id_e_t controller_id) : id(controller_id) {}

[[nodiscard]] uint16_t Controller::aPressed() const { return this->a; }

[[nodiscard]] uint16_t Controller::bPressed() const { return this->b; }

[[nodiscard]] uint16_t Controller::xPressed() const { return this->x; }

[[nodiscard]] uint16_t Controller::yPressed() const { return this->y; }

[[nodiscard]] uint16_t Controller::upPressed() const { return this->up; }

[[nodiscard]] uint16_t Controller::downPressed() const { return this->down; }

[[nodiscard]] uint16_t Controller::leftPressed() const { return this->left; }

[[nodiscard]] uint16_t Controller::rightPressed() const { return this->right; }

[[nodiscard]] uint16_t Controller::l1Pressed() const { return this->l1; }

[[nodiscard]] uint16_t Controller::l2Pressed() const { return this->l2; }

[[nodiscard]] uint16_t Controller::r1Pressed() const { return this->r1; }

[[nodiscard]] uint16_t Controller::r2Pressed() const { return this->r2; }

[[nodiscard]] double Controller::leftStickX() const { return this->lsX; }

[[nodiscard]] double Controller::leftStickY() const { return this->lsY; }

[[nodiscard]] double Controller::rightStickX() const { return this->rsX; }

[[nodiscard]] double Controller::rightStickY() const { return this->rsY; }

void Controller::setLine(uint8_t line, std::string str) {
  if (std::equal(text[line].begin(), text[line].end(), str.begin(), str.end())) {
    text[line] = str;
    textDirty[line] = true;
  }
}

void Controller::clearLine(uint8_t line) {
  if (!text.empty()) {
    text[line] = "";
    textDirty[line] = true;
  }
}

void Controller::rumble(const char *str) {
  this->enqueuedRumble = str;
}

bool Controller::isConnected() const { return pros::c::controller_is_connected(this->id); }

void Controller::update() {
  this->ticks++;
  if (!this->isConnected()) {
    this->resetState();
    return;
  }

  if (this->get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
    this->a++;
  } else {
    this->a = 0;
  }

  if (this->get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
    this->b++;
  } else {
    this->b = 0;
  }

  if (this->get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
    this->x++;
  } else {
    this->x = 0;
  }

  if (this->get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
    this->y++;
  } else {
    this->y = 0;
  }

  if (this->get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
    this->up++;
  } else {
    this->up = 0;
  }

  if (this->get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
    this->down++;
  } else {
    this->down = 0;
  }

  if (this->get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
    this->left++;
  } else {
    this->left = 0;
  }

  if (this->get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
    this->right++;
  } else {
    this->right = 0;
  }

  if (this->get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
    this->l1++;
  } else {
    this->l1 = 0;
  }

  if (this->get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
    this->l2++;
  } else {
    this->l2 = 0;
  }

  if (this->get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
    this->r1++;
  } else {
    this->r1 = 0;
  }

  if (this->get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
    this->r2++;
  } else {
    this->r2 = 0;
  }

  this->lsX = this->get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
  this->lsY = this->get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
  this->rsX = this->get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
  this->rsY = this->get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

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

  if (this->enqueuedRumble != nullptr && this->ticks % 10 == 0) {
    if (pros::c::controller_rumble(this->id, this->enqueuedRumble) == 1) {
      this->enqueuedRumble = nullptr;
    } else {
      logger::debug("Failed to send rumble. Trying again soon.");
    }
  }

  for (auto i = 0u; i < LINE_COUNT; ++i) {
    if (this->textDirty[i]) {
      bool success;
      if (!this->text[i].empty()) {
        success = pros::c::controller_set_text(this->id, i, 0, this->text[i].c_str()) == 1;
      } else {
        success = pros::c::controller_clear_line(this->id, i) == 1;
      }
      if (success) {
        this->textDirty[i] = false;
        break; // only one success per cycle
      }
    }
  }
}

void Controller::resetState() {
  this->a = 0;
  this->b = 0;
  this->x = 0;
  this->y = 0;

  this->up = 0;
  this->down = 0;
  this->left = 0;
  this->right = 0;

  this->l1 = 0;
  this->r1 = 0;
  this->l2 = 0;
  this->r2 = 0;
  this->lsX = 0.0;
  this->lsY = 0.0;
  this->rsX = 0.0;
  this->rsY = 0.0;
}

bool Controller::get_digital(pros::controller_digital_e_t button) {
  return pros::c::controller_get_digital(this->id, button) == 1;
}

double Controller::get_analog(pros::controller_analog_e_t stick) {
  double value = pros::c::controller_get_analog(this->id, stick);
  if (error::check(value)) return 0.0;
  return value;
}
} // namespace control::input
