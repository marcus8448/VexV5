#ifndef CONTROL_INPUT_RAW_RECORDING_HPP
#define CONTROL_INPUT_RAW_RECORDING_HPP

#include "controller.hpp"
#include "operator.hpp"
#include "pros/misc.h"
#include <cstdint>
#include <fstream>

namespace control::input {
/**
 * The default type of controller.
 * Updates the controller state based on the V5 controller input.
 */
class RawRecording : public Controller {
private:
  Controller *controller;
  std::ofstream output;

public:
  explicit RawRecording(Controller *controller = new Operator(), const char *name = "recording.v5r");

  [[nodiscard]] uint16_t a_pressed() const override;
  [[nodiscard]] uint16_t b_pressed() const override;
  [[nodiscard]] uint16_t x_pressed() const override;
  [[nodiscard]] uint16_t y_pressed() const override;

  [[nodiscard]] uint16_t up_pressed() const override;
  [[nodiscard]] uint16_t down_pressed() const override;
  [[nodiscard]] uint16_t left_pressed() const override;
  [[nodiscard]] uint16_t right_pressed() const override;

  [[nodiscard]] uint16_t l1_pressed() const override;
  [[nodiscard]] uint16_t l2_pressed() const override;
  [[nodiscard]] uint16_t r1_pressed() const override;
  [[nodiscard]] uint16_t r2_pressed() const override;

  [[nodiscard]] double left_stick_x() const override;
  [[nodiscard]] double left_stick_y() const override;
  [[nodiscard]] double right_stick_x() const override;
  [[nodiscard]] double right_stick_y() const override;

  [[nodiscard]] int16_t flywheel_speed() const override;
  void flywheel_speed(int16_t speed) override;

  void set_line(uint8_t line, uint8_t col, const char *str) override;
  void clear_line(uint8_t line) override;

  void rumble(const char *str) override;

  void update() override;
};
} // namespace control::input
#endif // CONTROL_INPUT_RAW_RECORDING_HPP
