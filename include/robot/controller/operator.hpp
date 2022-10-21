#ifndef ROBOT_CONTROLLER_OPERATOR_HPP
#define ROBOT_CONTROLLER_OPERATOR_HPP

#include "controller.hpp"
#include "pros/misc.hpp"
#include <cstdint>

namespace robot::controller {
/**
 * The default type of controller.
 * Updates the controller state based on the V5 controller input.
 */
class OpController : public Controller {
private:
  pros::Controller controller;

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

  double leftStickX = 0.0;
  double leftStickY = 0.0;
  double rightStickX = 0.0;
  double rightStickY = 0.0;

  double prevLeftStickX = 0.0;
  double prevLeftStickY = 0.0;
  double prevRightStickX = 0.0;
  double prevRightStickY = 0.0;

  double flywheelSpeed = 375.0;

public:
  explicit OpController(pros::Controller controller = pros::Controller(pros::E_CONTROLLER_MASTER));

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

  [[nodiscard]] double prev_left_stick_x() const override;
  [[nodiscard]] double prev_left_stick_y() const override;
  [[nodiscard]] double prev_right_stick_x() const override;
  [[nodiscard]] double prev_right_stick_y() const override;

  [[nodiscard]] double flywheel_speed() const override;
  void flywheel_speed(double speed) override;

  void set_line(uint8_t line, uint8_t col, const char *str) override;
  void clear_line(uint8_t line) override;

  void rumble(const char *str) override;

  void update() override;
};
} // namespace robot::controller
#endif // ROBOT_CONTROLLER_OPERATOR_HPP
