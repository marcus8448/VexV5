#include "control/input/raw_recording.hpp"
#include "filesystem.hpp"
#include <cstring>

namespace control::input {
RawRecording::RawRecording(Controller *controller, const char *name)
    : controller(controller), output(fs::open_indexed(name, std::ios::out | std::ios::binary)) {
  *this->output << "v5r\n";
}

[[nodiscard]] uint16_t RawRecording::aPressed() const { return this->controller->aPressed(); }

[[nodiscard]] uint16_t RawRecording::bPressed() const { return this->controller->bPressed(); }

[[nodiscard]] uint16_t RawRecording::xPressed() const { return this->controller->xPressed(); }

[[nodiscard]] uint16_t RawRecording::yPressed() const { return this->controller->yPressed(); }

[[nodiscard]] uint16_t RawRecording::upPressed() const { return this->controller->upPressed(); }

[[nodiscard]] uint16_t RawRecording::downPressed() const { return this->controller->downPressed(); }

[[nodiscard]] uint16_t RawRecording::leftPressed() const { return this->controller->leftPressed(); }

[[nodiscard]] uint16_t RawRecording::rightPressed() const { return this->controller->rightPressed(); }

[[nodiscard]] uint16_t RawRecording::l1Pressed() const { return this->controller->l1Pressed(); }

[[nodiscard]] uint16_t RawRecording::l2Pressed() const { return this->controller->l2Pressed(); }

[[nodiscard]] uint16_t RawRecording::r1Pressed() const { return this->controller->r1Pressed(); }

[[nodiscard]] uint16_t RawRecording::r2Pressed() const { return this->controller->r2Pressed(); }

[[nodiscard]] double RawRecording::leftStickX() const { return this->controller->leftStickX(); }

[[nodiscard]] double RawRecording::leftStickY() const { return this->controller->leftStickY(); }

[[nodiscard]] double RawRecording::rightStickX() const { return this->controller->rightStickX(); }

[[nodiscard]] double RawRecording::rightStickY() const { return this->controller->rightStickY(); }

[[nodiscard]] int16_t RawRecording::speedSetting() const { return this->controller->speedSetting(); }

void RawRecording::setSpeedSetting(int16_t speed) { this->controller->setSpeedSetting(speed); }

void RawRecording::setLine(uint8_t line, uint8_t col, const char *str) { this->controller->setLine(line, col, str); }

void RawRecording::clearLine(uint8_t line) { this->controller->clearLine(line); }

void RawRecording::rumble(const char *str) { this->controller->rumble(str); }

void RawRecording::update() {
  if (!this->output->is_open())
    return;

  this->controller->update();

  const uint32_t SIZE = 2 + (sizeof(float) * 4);
  uint8_t buf[SIZE];

  buf[0] = 0;
  buf[1] = 0;

  if (this->controller->aPressed())
    buf[0] |= 0b00000001;
  if (this->controller->bPressed())
    buf[0] |= 0b00000010;
  if (this->controller->xPressed())
    buf[0] |= 0b00000100;
  if (this->controller->yPressed())
    buf[0] |= 0b00001000;
  if (this->controller->upPressed())
    buf[0] |= 0b00010000;
  if (this->controller->downPressed())
    buf[0] |= 0b00100000;
  if (this->controller->leftPressed())
    buf[0] |= 0b01000000;
  if (this->controller->rightPressed())
    buf[0] |= 0b10000000;
  if (this->controller->l1Pressed())
    buf[1] |= 0b00000001;
  if (this->controller->l2Pressed())
    buf[1] |= 0b00000010;
  if (this->controller->r1Pressed())
    buf[1] |= 0b00000100;
  if (this->controller->r2Pressed())
    buf[1] |= 0b00001000;
  float src;
  src = static_cast<float>(this->controller->leftStickX());
  std::memcpy(&buf[2 + sizeof(float) * 0], &src, sizeof(float));
  src = static_cast<float>(this->controller->leftStickY());
  std::memcpy(&buf[2 + sizeof(float) * 1], &src, sizeof(float));
  src = static_cast<float>(this->controller->rightStickX());
  std::memcpy(&buf[2 + sizeof(float) * 2], &src, sizeof(float));
  src = static_cast<float>(this->controller->rightStickY());
  std::memcpy(&buf[2 + sizeof(float) * 3], &src, sizeof(float));

  if (this->controller->xPressed() >= 50) {
    this->output->flush();
    this->output->close();
  }
}
} // namespace control::input
