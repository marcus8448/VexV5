#include "robot/controller/raw_recording.hpp"
#include "fs/filesystem.hpp"
#include <cstring>

namespace robot::controller {
RawRecording::RawRecording(Controller *controller, const char *name)
    : controller(controller), output(fs::open_indexed(name, std::ios::out | std::ios::binary)) {
  this->output << "v5r\n";
}

[[nodiscard]] uint16_t RawRecording::a_pressed() const { return this->controller->a_pressed(); }

[[nodiscard]] uint16_t RawRecording::b_pressed() const { return this->controller->b_pressed(); }

[[nodiscard]] uint16_t RawRecording::x_pressed() const { return this->controller->x_pressed(); }

[[nodiscard]] uint16_t RawRecording::y_pressed() const { return this->controller->y_pressed(); }

[[nodiscard]] uint16_t RawRecording::up_pressed() const { return this->controller->up_pressed(); }

[[nodiscard]] uint16_t RawRecording::down_pressed() const { return this->controller->down_pressed(); }

[[nodiscard]] uint16_t RawRecording::left_pressed() const { return this->controller->left_pressed(); }

[[nodiscard]] uint16_t RawRecording::right_pressed() const { return this->controller->right_pressed(); }

[[nodiscard]] uint16_t RawRecording::l1_pressed() const { return this->controller->l1_pressed(); }

[[nodiscard]] uint16_t RawRecording::l2_pressed() const { return this->controller->l2_pressed(); }

[[nodiscard]] uint16_t RawRecording::r1_pressed() const { return this->controller->r1_pressed(); }

[[nodiscard]] uint16_t RawRecording::r2_pressed() const { return this->controller->r2_pressed(); }

[[nodiscard]] double RawRecording::left_stick_x() const { return this->controller->left_stick_x(); }

[[nodiscard]] double RawRecording::left_stick_y() const { return this->controller->left_stick_y(); }

[[nodiscard]] double RawRecording::right_stick_x() const { return this->controller->right_stick_x(); }

[[nodiscard]] double RawRecording::right_stick_y() const { return this->controller->right_stick_y(); }

[[nodiscard]] int16_t RawRecording::flywheel_speed() const { return this->controller->flywheel_speed(); }

void RawRecording::flywheel_speed(int16_t speed) { this->controller->flywheel_speed(speed); }

void RawRecording::set_line(uint8_t line, uint8_t col, const char *str) { this->controller->set_line(line, col, str); }

void RawRecording::clear_line(uint8_t line) { this->controller->clear_line(line); }

void RawRecording::rumble(const char *str) { this->controller->rumble(str); }

void RawRecording::update() {
  if (!this->output.is_open())
    return;

  this->controller->update();

  const uint32_t SIZE = 2 + (sizeof(float) * 4);
  uint8_t buf[SIZE];

  buf[0] = 0;
  buf[1] = 0;

  if (this->controller->a_pressed())
    buf[0] |= 0b00000001;
  if (this->controller->b_pressed())
    buf[0] |= 0b00000010;
  if (this->controller->x_pressed())
    buf[0] |= 0b00000100;
  if (this->controller->y_pressed())
    buf[0] |= 0b00001000;
  if (this->controller->up_pressed())
    buf[0] |= 0b00010000;
  if (this->controller->down_pressed())
    buf[0] |= 0b00100000;
  if (this->controller->left_pressed())
    buf[0] |= 0b01000000;
  if (this->controller->right_pressed())
    buf[0] |= 0b10000000;
  if (this->controller->l1_pressed())
    buf[1] |= 0b00000001;
  if (this->controller->l2_pressed())
    buf[1] |= 0b00000010;
  if (this->controller->r1_pressed())
    buf[1] |= 0b00000100;
  if (this->controller->r2_pressed())
    buf[1] |= 0b00001000;
  float src;
  src = static_cast<float>(this->controller->left_stick_x());
  std::memcpy(&buf[2 + sizeof(float) * 0], &src, sizeof(float));
  src = static_cast<float>(this->controller->left_stick_y());
  std::memcpy(&buf[2 + sizeof(float) * 1], &src, sizeof(float));
  src = static_cast<float>(this->controller->right_stick_x());
  std::memcpy(&buf[2 + sizeof(float) * 2], &src, sizeof(float));
  src = static_cast<float>(this->controller->right_stick_y());
  std::memcpy(&buf[2 + sizeof(float) * 3], &src, sizeof(float));

  if (this->controller->x_pressed() >= 50) {
    this->output.flush();
    this->output.close();
  }
}
} // namespace robot::controller
