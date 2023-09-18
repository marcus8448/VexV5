#ifndef CONTROL_INPUT_OPERATOR_HPP
#define CONTROL_INPUT_OPERATOR_HPP

#include "controller.hpp"
#include "pros/misc.h"
#include <array>
#include <cstdint>

namespace control::input {
/**
 * The default type of controller.
 * Updates the controller state based on the V5 controller input.
 */
class Operator : public Controller {
private:
  pros::controller_id_e_t controllerId;

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

  uint32_t ticks = 0;

  std::array<const char *, LINE_COUNT> text = {};
  std::array<bool, LINE_COUNT> textDirty = {};

  const char *enqueuedRumble = nullptr;

public:
  explicit Operator(pros::controller_id_e_t controller_id = pros::E_CONTROLLER_MASTER);

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

  void setLine(uint8_t line, const char *str) override;
  void clearLine(uint8_t line) override;

  void rumble(const char *str) override;

  void update() override;
};
} // namespace control::input
#endif // CONTROL_INPUT_OPERATOR_HPP
