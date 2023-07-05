#ifndef CONTROL_INPUT_RAW_REPLAY_HPP
#define CONTROL_INPUT_RAW_REPLAY_HPP

#include "controller.hpp"
#include "pros/misc.h"
#include <cstdint>
#include <fstream>

namespace control::input {
/**
 * The default type of controller.
 * Updates the controller state based on the V5 controller input.
 */
class RawReplay : public Controller {
private:
  std::ifstream *input;

  uint16_t a = 0;
  uint16_t b = 0;
  uint16_t x = 0;
  uint16_t y = 0;

  uint16_t up = 0;
  uint16_t down = 0;
  uint16_t left = 0;
  uint16_t right = 0;

  uint16_t l1 = 0;
  uint16_t l2 = 0;
  uint16_t r1 = 0;
  uint16_t r2 = 0;

  double lsX = 0.0;
  double lsY = 0.0;
  double rsX = 0.0;
  double rsY = 0.0;

  int16_t flywheelSpeed = 7100;
  uint32_t ticks = 0;

public:
  explicit RawReplay(const char *name);

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
#endif // CONTROL_INPUT_RAW_REPLAY_HPP
