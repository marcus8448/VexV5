#ifndef CONTROL_INPUT_CONTROLLER_HPP
#define CONTROL_INPUT_CONTROLLER_HPP

#include "pros/misc.h"
#include <array>
#include <cstdint>

namespace control::input {
/**
 * A source of driver input.
 */
class Controller {
public:
  static constexpr double JOYSTICK_MAX = 127.0;
  static constexpr std::size_t LINE_COUNT = 3;

private:
  pros::controller_id_e_t id;

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
  explicit Controller(pros::controller_id_e_t controller_id);
  ~Controller() = default;
  Controller(const Controller &) = delete;
  Controller &operator=(const Controller &) = delete;

  /**
   * Returns a number greater than zero if the a button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] uint16_t aPressed() const;

  /**
   * Returns a number greater than zero if the b button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] uint16_t bPressed() const;

  /**
   * Returns a number greater than zero if the x button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] uint16_t xPressed() const;

  /**
   * Returns a number greater than zero if the y button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] uint16_t yPressed() const;

  /**
   * Returns a number greater than zero if the up button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] uint16_t upPressed() const;

  /**
   * Returns a number greater than zero if the down button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] uint16_t downPressed() const;

  /**
   * Returns a number greater than zero if the left button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] uint16_t leftPressed() const;

  /**
   * Returns a number greater than zero if the right button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] uint16_t rightPressed() const;

  /**
   * Returns a number greater than zero if the l1 button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] uint16_t l1Pressed() const;

  /**
   * Returns a number greater than zero if the l2 button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] uint16_t l2Pressed() const;

  /**
   * Returns a number greater than zero if the r1 button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] uint16_t r1Pressed() const;

  /**
   * Returns a number greater than zero if the r2 button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] uint16_t r2Pressed() const;

  /**
   * Sets the row on the V5 controller to the specified text.
   * @param row The row to set [0-2].
   * @param str The text to write.
   */
  void setLine(uint8_t row, const char *str);

  /**
   * Blanks the specified row of text on the controller.
   * @param row The row to blank.
   */
  void clearLine(uint8_t row);

  /**
   * Rumbles the controller with a specific pattern. Repeats indefinitely.
   * The patten is set by morse code. ('-' = long, '.' = short, ' ' = rest)
   * @param str the morse code pattern to use.
   */
  void rumble(const char *str);

  /**
   * Returns the position of the left stick on the x-axis.
   * @return the position of the left stick on the x-axis.
   */
  [[nodiscard]] double leftStickX() const;

  /**
   * Returns the position of the left stick on the y-axis.
   * @return the position of the left stick on the y-axis.
   */
  [[nodiscard]] double leftStickY() const;

  /**
   * Returns the position of the right stick on the x-axis.
   * @return the position of the right stick on the x-axis.
   */
  [[nodiscard]] double rightStickX() const;

  /**
   * Returns the position of the right stick on the y-axis.
   * @return the position of the right stick on the y-axis.
   */
  [[nodiscard]] double rightStickY() const;

  [[nodiscard]] bool isConnected() const;

  /**
   * Updates the controller state.
   */
  void update();

private:
  void resetState();
  inline bool get_digital(pros::controller_digital_e_t button);
  inline double get_analog(pros::controller_analog_e_t stick);
};
} // namespace control::input

#endif // CONTROL_INPUT_CONTROLLER_HPP
