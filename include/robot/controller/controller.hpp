#ifndef ROBOT_CONTROLLER_CONTROLLER_HPP
#define ROBOT_CONTROLLER_CONTROLLER_HPP

#include <cstdint>

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
  [[nodiscard]] virtual uint16_t a_pressed() const = 0;

  /**
   * Returns a number greater than zero if the b button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t b_pressed() const = 0;

  /**
   * Returns a number greater than zero if the x button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t x_pressed() const = 0;

  /**
   * Returns a number greater than zero if the y button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t y_pressed() const = 0;

  /**
   * Returns a number greater than zero if the up button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t up_pressed() const = 0;

  /**
   * Returns a number greater than zero if the down button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t down_pressed() const = 0;

  /**
   * Returns a number greater than zero if the left button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t left_pressed() const = 0;

  /**
   * Returns a number greater than zero if the right button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t right_pressed() const = 0;

  /**
   * Returns a number greater than zero if the l1 button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t l1_pressed() const = 0;

  /**
   * Returns a number greater than zero if the l2 button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t l2_pressed() const = 0;

  /**
   * Returns a number greater than zero if the r1 button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t r1_pressed() const = 0;

  /**
   * Returns a number greater than zero if the r2 button is pressed.
   * @retrun if the a button is pressed.
   */
  [[nodiscard]] virtual uint16_t r2_pressed() const = 0;

  /**
   * Returns the current speed motors should run at when activated by a button (digital input).
   * @return the current speed motors should run at when activated by a button (digital input).
   */
  [[nodiscard]] virtual double flywheel_speed() const = 0;

  /**
   * Sets the speed motors should run at when activated by a button (digital input).
   * @param flywheelSpeed the speed motors should run at when activated by a button (digital input).
   */
  virtual void flywheel_speed(double flywheelSpeed) = 0;

  /**
   * Sets the row on the V5 controller to the specified text.
   * @param row The row to set [0-2].
   * @param col The column to start at [0-14].
   * @param str The text to write.
   */
  virtual void set_line(uint8_t row, uint8_t col, const char *str) = 0;

  /**
   * Blanks the specified row of text on the controller.
   * @param row The row to blank.
   */
  virtual void clear_line(uint8_t row) = 0;

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
  [[nodiscard]] virtual double left_stick_x() const = 0;

  /**
   * Returns the position of the left stick on the y-axis.
   * @return the position of the left stick on the y-axis.
   */
  [[nodiscard]] virtual double left_stick_y() const = 0;

  /**
   * Returns the position of the right stick on the x-axis.
   * @return the position of the right stick on the x-axis.
   */
  [[nodiscard]] virtual double right_stick_x() const = 0;

  /**
   * Returns the position of the right stick on the y-axis.
   * @return the position of the right stick on the y-axis.
   */
  [[nodiscard]] virtual double right_stick_y() const = 0;

  /**
   * Returns the previous position of the left stick on the x-axis.
   * @return the previous position of the left stick on the x-axis.
   */
  [[nodiscard]] virtual double prev_left_stick_x() const = 0;

  /**
   * Returns the previous position of the left stick on the y-axis.
   * @return the previous position of the left stick on the y-axis.
   */
  [[nodiscard]] virtual double prev_left_stick_y() const = 0;

  /**
   * Returns the previous position of the right stick on the x-axis.
   * @return the previous position of the right stick on the x-axis.
   */
  [[nodiscard]] virtual double prev_right_stick_x() const = 0;

  /**
   * Returns the previous position of the right stick on the y-axis.
   * @return the previous position of the right stick on the y-axis.
   */
  [[nodiscard]] virtual double prev_right_stick_y() const = 0;

  /**
   * Updates the controller state.
   */
  virtual void update() = 0;
};

/**
 * Represents objects that change state based on the controller.
 * Used during the operator control period.
 */
class Updatable {
public:
  /**
   * Updates this part of the robot based on the controller's state.
   * @param controller the controller of the robot.
   */
  virtual void update(Controller *controller) = 0;
};

#endif // ROBOT_CONTROLLER_CONTROLLER_HPP
