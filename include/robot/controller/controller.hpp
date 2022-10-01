#ifndef ROBOT_CONTROLLER_CONTROLLER_HPP
#define ROBOT_CONTROLLER_CONTROLLER_HPP

#include <cstdint>

class Controller {
public:
  virtual ~Controller() = default;

  virtual uint16_t a_pressed() = 0;
  virtual uint16_t b_pressed() = 0;
  virtual uint16_t x_pressed() = 0;
  virtual uint16_t y_pressed() = 0;

  virtual uint16_t up_pressed() = 0;
  virtual uint16_t down_pressed() = 0;
  virtual uint16_t left_pressed() = 0;
  virtual uint16_t right_pressed() = 0;

  virtual uint16_t l1_pressed() = 0;
  virtual uint16_t l2_pressed() = 0;
  virtual uint16_t r1_pressed() = 0;
  virtual uint16_t r2_pressed() = 0;

  virtual uint8_t digital_speed() = 0;
  virtual void digital_speed(uint8_t digitalSpeed) = 0;

  virtual void set_line(uint8_t line, uint8_t col, const char *str) = 0;
  virtual void clear_line(uint8_t line) = 0;

  virtual void rumble(const char *str) = 0;

  virtual double left_stick_x() = 0;
  virtual double left_stick_y() = 0;
  virtual double right_stick_x() = 0;
  virtual double right_stick_y() = 0;

  virtual double prev_left_stick_x() = 0;
  virtual double prev_left_stick_y() = 0;
  virtual double prev_right_stick_x() = 0;
  virtual double prev_right_stick_y() = 0;

  virtual void update() = 0;
};

class Updatable {
public:
  virtual void update(Controller *controller) = 0;
};

#endif // ROBOT_CONTROLLER_CONTROLLER_HPP
