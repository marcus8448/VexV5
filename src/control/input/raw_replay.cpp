#include "control/input/raw_replay.hpp"
#include "debug/logger.hpp"
#include "filesystem.hpp"
#include <cstring>

namespace control::input {
RawReplay::RawReplay(const char *name) : input(fs::open(name, std::ios_base::in | std::ios_base::binary)) {
  if (!input->is_open() || input->bad()) {
    input->close();
    error("Failed to read recording.");
    return;
  }

  char buf[4];
  input->read(buf, 4);
  if (input->eof() || strcmp(buf, "v5r\n") != 0) {
    input->close();
    error("Recording in invalid format! %s", buf);
    return;
  }
}

[[nodiscard]] uint16_t RawReplay::aPressed() const { return this->a; }

[[nodiscard]] uint16_t RawReplay::bPressed() const { return this->b; }

[[nodiscard]] uint16_t RawReplay::xPressed() const { return this->x; }

[[nodiscard]] uint16_t RawReplay::yPressed() const { return this->y; }

[[nodiscard]] uint16_t RawReplay::upPressed() const { return this->up; }

[[nodiscard]] uint16_t RawReplay::downPressed() const { return this->down; }

[[nodiscard]] uint16_t RawReplay::leftPressed() const { return this->left; }

[[nodiscard]] uint16_t RawReplay::rightPressed() const { return this->right; }

[[nodiscard]] uint16_t RawReplay::l1Pressed() const { return this->l1; }

[[nodiscard]] uint16_t RawReplay::l2Pressed() const { return this->l2; }

[[nodiscard]] uint16_t RawReplay::r1Pressed() const { return this->r1; }

[[nodiscard]] uint16_t RawReplay::r2Pressed() const { return this->r2; }

[[nodiscard]] double RawReplay::leftStickX() const { return this->lsX; }

[[nodiscard]] double RawReplay::leftStickY() const { return this->lsY; }

[[nodiscard]] double RawReplay::rightStickX() const { return this->rsX; }

[[nodiscard]] double RawReplay::rightStickY() const { return this->rsY; }

[[nodiscard]] int16_t RawReplay::speedSetting() const { return this->flywheelSpeed; }

void RawReplay::setSpeedSetting(int16_t speed) { this->flywheelSpeed = speed; }

void RawReplay::setLine(uint8_t line, uint8_t col, const char *str) {}

void RawReplay::clearLine(uint8_t line) {}

void RawReplay::rumble(const char *str) {}

void RawReplay::update() {
  if (this->input->eof())
    return;

  this->ticks++;
  const uint32_t SIZE = 2 + (sizeof(float) * 4);
  char buf[SIZE];
  this->input->read(buf, SIZE);

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

  std::memcpy(&this->lsX, &buf[2 + sizeof(float) * 0], sizeof(float));
  std::memcpy(&this->lsY, &buf[2 + sizeof(float) * 1], sizeof(float));
  std::memcpy(&this->rsX, &buf[2 + sizeof(float) * 2], sizeof(float));
  std::memcpy(&this->rsY, &buf[2 + sizeof(float) * 3], sizeof(float));

  if (this->rightPressed()) {
    this->setSpeedSetting(static_cast<int16_t>(std::min(this->speedSetting() + 100, 12000)));
  } else if (this->leftPressed()) {
    this->setSpeedSetting(static_cast<int16_t>(std::max(this->speedSetting() - 100, 4000)));
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
} // namespace control::input
