#ifndef CONTROL_INPUT_CONTROLLER_HPP
#define CONTROL_INPUT_CONTROLLER_HPP

#include <cstdint>

namespace control::input {
/**
 * A source of driver input.
 */
class Controller {
public:
  virtual ~Controller() = default;

  /**
   * Returns a number greater than zero if the a button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t aPressed() const = 0;

  /**
   * Returns a number greater than zero if the b button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t bPressed() const = 0;

  /**
   * Returns a number greater than zero if the x button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t xPressed() const = 0;

  /**
   * Returns a number greater than zero if the y button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t yPressed() const = 0;

  /**
   * Returns a number greater than zero if the up button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t upPressed() const = 0;

  /**
   * Returns a number greater than zero if the down button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t downPressed() const = 0;

  /**
   * Returns a number greater than zero if the left button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t leftPressed() const = 0;

  /**
   * Returns a number greater than zero if the right button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t rightPressed() const = 0;

  /**
   * Returns a number greater than zero if the l1 button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t l1Pressed() const = 0;

  /**
   * Returns a number greater than zero if the l2 button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t l2Pressed() const = 0;

  /**
   * Returns a number greater than zero if the r1 button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t r1Pressed() const = 0;

  /**
   * Returns a number greater than zero if the r2 button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t r2Pressed() const = 0;

  /**
   * Returns the current speed motors should run at when activated by a button (digital input).
   * @return the current speed motors should run at when activated by a button (digital input).
   */
  [[nodiscard]] virtual int16_t speedSetting() const = 0;

  /**
   * Sets the speed motors should run at when activated by a button (digital input).
   * @param flywheelSpeed the speed motors should run at when activated by a button (digital input).
   */
  virtual void setSpeedSetting(int16_t flywheelSpeed) = 0;

  /**
   * Sets the row on the V5 controller to the specified text.
   * @param row The row to set [0-2].
   * @param col The column to start at [0-14].
   * @param str The text to write.
   */
  virtual void setLine(uint8_t row, uint8_t col, const char *str) = 0;

  /**
   * Blanks the specified row of text on the controller.
   * @param row The row to blank.
   */
  virtual void clearLine(uint8_t row) = 0;

  /**
   * Rumbles the controller with a specific pattern. Repeats indefinitely.
   * The patten is set by morse code. ('-' = long, '.' = short, ' ' = rest)
   * @param str the morse code pattern to use.
   */
  virtual void rumble(const char *str) = 0;

  /**
   * Returns the position of the left stick on the x-axis.
   * @return the position of the left stick on the x-axis.
   */
  [[nodiscard]] virtual double leftStickX() const = 0;

  /**
   * Returns the position of the left stick on the y-axis.
   * @return the position of the left stick on the y-axis.
   */
  [[nodiscard]] virtual double leftStickY() const = 0;

  /**
   * Returns the position of the right stick on the x-axis.
   * @return the position of the right stick on the x-axis.
   */
  [[nodiscard]] virtual double rightStickX() const = 0;

  /**
   * Returns the position of the right stick on the y-axis.
   * @return the position of the right stick on the y-axis.
   */
  [[nodiscard]] virtual double rightStickY() const = 0;

  /**
   * Updates the controller state.
   */
  virtual void update() = 0;
};
} // namespace control::input

#endif // CONTROL_INPUT_CONTROLLER_HPP