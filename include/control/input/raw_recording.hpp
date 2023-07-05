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
  std::ofstream *output;

public:
  explicit RawRecording(Controller *controller = new Operator(), const char *name = "recording.v5r");

  [[nodiscard]] uint16_t aPressed() const override;
  [[nodiscard]] uint16_t bPressed() const override;
  [[nodiscard]] uint16_t xPressed() const override;
  [[nodiscard]] uint16_t yPressed() const override;

  [[nodiscard]] uint16_t upPressed() const override;
  [[nodiscard]] uint16_t downPressed() const override;
  [[nodiscard]] uint16_t leftPressed() const override;
  [[nodiscard]] uint16_t rightPressed() const override;

  [[nodiscard]] uint16_t l1Pressed() const override;
  [[nodiscard]] uint16_t l2Pressed() const override;
  [[nodiscard]] uint16_t r1Pressed() const override;
  [[nodiscard]] uint16_t r2Pressed() const override;

  [[nodiscard]] double leftStickX() const override;
  [[nodiscard]] double leftStickY() const override;
  [[nodiscard]] double rightStickX() const override;
  [[nodiscard]] double rightStickY() const override;

  [[nodiscard]] int16_t speedSetting() const override;
  void setSpeedSetting(int16_t speed) override;

  void setLine(uint8_t line, uint8_t col, const char *str) override;
  void clearLine(uint8_t line) override;

  void rumble(const char *str) override;

  void update() override;
};
} // namespace control::input
#endif // CONTROL_INPUT_RAW_RECORDING_HPP
