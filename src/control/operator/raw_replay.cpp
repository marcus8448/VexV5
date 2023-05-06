#include "control/operator/raw_replay.hpp"
#include "debug/error.hpp"
#include "debug/logger.hpp"
#include "fs/filesystem.hpp"
#include <cstring>

namespace robot::controller {
RawReplay::RawReplay(const char *name) : input(fs::open(name, std::ios_base::in | std::ios_base::binary)) {
  if (!input.is_open() || input.bad()) {
    input.close();
    error("Failed to read recording.");
    return;
  }

  char buf[4];
  input.read(buf, 4);
  if (input.eof() || strcmp(buf, "v5r\n") != 0) {
    input.close();
    error("Recording in invalid format!");
    return;
  }
}

[[nodiscard]] uint16_t RawReplay::a_pressed() const { return this->a; }

[[nodiscard]] uint16_t RawReplay::b_pressed() const { return this->b; }

[[nodiscard]] uint16_t RawReplay::x_pressed() const { return this->x; }

[[nodiscard]] uint16_t RawReplay::y_pressed() const { return this->y; }

[[nodiscard]] uint16_t RawReplay::up_pressed() const { return this->up; }

[[nodiscard]] uint16_t RawReplay::down_pressed() const { return this->down; }

[[nodiscard]] uint16_t RawReplay::left_pressed() const { return this->left; }

[[nodiscard]] uint16_t RawReplay::right_pressed() const { return this->right; }

[[nodiscard]] uint16_t RawReplay::l1_pressed() const { return this->l1; }

[[nodiscard]] uint16_t RawReplay::l2_pressed() const { return this->l2; }

[[nodiscard]] uint16_t RawReplay::r1_pressed() const { return this->r1; }

[[nodiscard]] uint16_t RawReplay::r2_pressed() const { return this->r2; }

[[nodiscard]] double RawReplay::left_stick_x() const { return this->leftStickX; }

[[nodiscard]] double RawReplay::left_stick_y() const { return this->leftStickY; }

[[nodiscard]] double RawReplay::right_stick_x() const { return this->rightStickX; }

[[nodiscard]] double RawReplay::right_stick_y() const { return this->rightStickY; }

[[nodiscard]] int16_t RawReplay::flywheel_speed() const { return this->flywheelSpeed; }

void RawReplay::flywheel_speed(int16_t speed) { this->flywheelSpeed = speed; }

void RawReplay::set_line(uint8_t line, uint8_t col, const char *str) {}

void RawReplay::clear_line(uint8_t line) {}

void RawReplay::rumble(const char *str) {}

void RawReplay::update() {
  if (this->input.eof())
    return;

  this->ticks++;
  const uint32_t SIZE = 2 + (sizeof(float) * 4);
  char buf[SIZE];
  this->input.read(buf, SIZE);

  if ((buf[0] & 0b00000001) != 0) {
    this->a++;
  } else {
    this->a = 0;
  }

  if ((buf[0] & 0b00000010) != 0) {
    this->b++;
  } else {
    this->b = 0;
  }

  if ((buf[0] & 0b00000100) != 0) {
    this->x++;
  } else {
    this->x = 0;
  }

  if ((buf[0] & 0b00001000) != 0) {
    this->y++;
  } else {
    this->y = 0;
  }

  if ((buf[0] & 0b00010000) != 0) {
    this->up++;
  } else {
    this->up = 0;
  }

  if ((buf[0] & 0b00100000) != 0) {
    this->down++;
  } else {
    this->down = 0;
  }

  if ((buf[0] & 0b01000000) != 0) {
    this->left++;
  } else {
    this->left = 0;
  }

  if ((buf[0] & 0b10000000) != 0) {
    this->right++;
  } else {
    this->right = 0;
  }

  if ((buf[1] & 0b00000001) != 0) {
    this->l1++;
  } else {
    this->l1 = 0;
  }

  if ((buf[1] & 0b00000010) != 0) {
    this->l2++;
  } else {
    this->l2 = 0;
  }

  if ((buf[1] & 0b00000100) != 0) {
    this->r1++;
  } else {
    this->r1 = 0;
  }

  if ((buf[1] & 0b00001000) != 0) {
    this->r2++;
  } else {
    this->r2 = 0;
  }

  std::memcpy(&this->leftStickX, &buf[2 + sizeof(float) * 0], sizeof(float));
  std::memcpy(&this->leftStickY, &buf[2 + sizeof(float) * 1], sizeof(float));
  std::memcpy(&this->rightStickX, &buf[2 + sizeof(float) * 2], sizeof(float));
  std::memcpy(&this->rightStickY, &buf[2 + sizeof(float) * 3], sizeof(float));

  if (this->right_pressed()) {
    this->flywheel_speed(static_cast<int16_t>(std::min(this->flywheel_speed() + 100, 12000)));
  } else if (this->left_pressed()) {
    this->flywheel_speed(static_cast<int16_t>(std::max(this->flywheel_speed() - 100, 4000)));
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
}
} // namespace robot::controller
