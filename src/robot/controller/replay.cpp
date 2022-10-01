#include "robot/controller/replay.hpp"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include <cstring>
#include <fstream>

namespace robot::controller {
ReplayController::ReplayController(const char *filename) {
  auto controller = pros::Controller(pros::E_CONTROLLER_MASTER);
  while (!pros::usd::is_installed()) {
    controller.set_text(2, 0, "Missing microSD!");
    pros::delay(250);
  }
  controller.clear_line(2);

  this->inf = std::ifstream(filename, std::ios::in | std::ios::binary);
  this->inf >> this->type;
}

uint16_t ReplayController::a_pressed() {
  return this->a;
}

uint16_t ReplayController::b_pressed() {
  return this->b;
}

uint16_t ReplayController::x_pressed() {
  return this->x;
}

uint16_t ReplayController::y_pressed() {
  return this->y;
}

uint16_t ReplayController::up_pressed() {
  return this->up;
}

uint16_t ReplayController::down_pressed() {
  return this->down;
}

uint16_t ReplayController::left_pressed() {
  return this->left;
}

uint16_t ReplayController::right_pressed() {
  return this->right;
}

uint16_t ReplayController::l1_pressed() {
  return this->l1;
}

uint16_t ReplayController::l2_pressed() {
  return this->l2;
}

uint16_t ReplayController::r1_pressed() {
  return this->r1;
}

uint16_t ReplayController::r2_pressed() {
  return this->r2;
}

uint8_t ReplayController::digital_speed() {
  return this->digitalSpeed;
}

void ReplayController::digital_speed(uint8_t speed) {
  this->digitalSpeed = speed;
}

void ReplayController::set_line(uint8_t line, uint8_t col, const char *str) {}

void ReplayController::clear_line(uint8_t line) {}

void ReplayController::rumble(const char *str) {}

double ReplayController::left_stick_x() {
  return this->leftStickX;
}

double ReplayController::left_stick_y() {
  return this->leftStickY;
}

double ReplayController::right_stick_x() {
  return this->rightStickX;
}

double ReplayController::right_stick_y() {
  return this->rightStickY;
}

double ReplayController::prev_left_stick_x() {
  return this->prevLeftStickX;
}

double ReplayController::prev_left_stick_y() {
  return this->prevLeftStickY;
}

double ReplayController::prev_right_stick_x() {
  return this->prevRightStickX;
}

double ReplayController::prev_right_stick_y() {
  return this->prevRightStickY;
}

void ReplayController::update() {
  if (this->inf.peek() == std::char_traits<char>::eof()) {
    return;
  }

  char c = '*';

  bool exit = false;
  while (!exit) {
    inf >> c;
    switch (c) {
    case 'a':a = !a;
      break;
    case 'b':b = !b;
      break;
    case 'x':x = !x;
      break;
    case 'y':y = !y;
      break;
    case 'u':up = !up;
      break;
    case 'd':down = !down;
      break;
    case 'l':left = !left;
      break;
    case 'r':right = !right;
      break;
    case '!':l1 = !l1;
      break;
    case '@':l2 = !l2;
      break;
    case '#':r1 = !r1;
      break;
    case '$':r2 = !r2;
      break;
    case '*':exit = true;
      break;
    default:printf("Invalid code! %c", c);
      inf.close();
      return;
    }
  }
  uint64_t value;

  inf >> value;
  std::memcpy(&leftStickX, &value, sizeof(value));
  inf >> value;
  std::memcpy(&leftStickY, &value, sizeof(value));
  inf >> value;
  std::memcpy(&rightStickX, &value, sizeof(value));
  inf >> value;
  std::memcpy(&rightStickY, &value, sizeof(value));
}
}