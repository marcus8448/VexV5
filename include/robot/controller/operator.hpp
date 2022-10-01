#ifndef ROBOT_CONTROLLER_OPERATOR_HPP
#define ROBOT_CONTROLLER_OPERATOR_HPP

#include "controller.hpp"
#include "pros/misc.hpp"
#include <cstdint>

namespace robot::controller {
class OpController : public Controller {
public:
  pros::Controller controller;

private:
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

  uint8_t digitalSpeed = 127;

public:
  explicit OpController(pros::Controller controller = pros::Controller(pros::E_CONTROLLER_MASTER));

  uint16_t a_pressed() override;
  uint16_t b_pressed() override;
  uint16_t x_pressed() override;
  uint16_t y_pressed() override;

  uint16_t up_pressed() override;
  uint16_t down_pressed() override;
  uint16_t left_pressed() override;
  uint16_t right_pressed() override;

  uint16_t l1_pressed() override;
  uint16_t l2_pressed() override;
  uint16_t r1_pressed() override;
  uint16_t r2_pressed() override;

  double left_stick_x() override;
  double left_stick_y() override;
  double right_stick_x() override;
  double right_stick_y() override;

  double prev_left_stick_x() override;
  double prev_left_stick_y() override;
  double prev_right_stick_x() override;
  double prev_right_stick_y() override;

  uint8_t digital_speed() override;
  void digital_speed(uint8_t speed) override;

  void set_line(uint8_t line, uint8_t col, const char *str) override;
  void clear_line(uint8_t line) override;

  void rumble(const char *str) override;

  void update() override;
};
} // namespace robot::controller
#endif // ROBOT_CONTROLLER_OPERATOR_HPP
