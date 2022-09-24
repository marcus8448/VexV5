#ifndef ROBOT_HPP
#define ROBOT_HPP

#include "pros/misc.hpp"
#include "pros/motors.hpp"

class Resettable {
public:
  virtual void stop() {};
  virtual void reset() {};
};

class Targeting {
public:
  virtual bool is_offset_within(double distance) = 0;
};

class Positioned {
public:
  virtual double get_position() = 0;
  virtual double get_target_position() = 0;
};

class DualPositioned : public Positioned {
public:
  virtual double get_left_position() = 0;
  virtual double get_right_position() = 0;
  virtual double get_left_target_position() = 0;
  virtual double get_right_target_position() = 0;
  double get_position() override;
  double get_target_position() override;
};

class Controller : public Resettable {
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

  virtual unsigned char digital_speed() = 0;
  virtual void digital_speed(unsigned char digitalSpeed) = 0;

  virtual void set_line(unsigned char line, unsigned char col, const char *str) = 0;
  virtual void clear_line(unsigned char line) = 0;

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

  void reset() override = 0;
  void stop() override = 0;
};

class Updatable {
public:
  virtual void update(Controller *controller) {};
};

class Drivetrain : public Resettable, public Targeting, public Updatable {
public:
  pros::Motor *rightFront;
  pros::Motor *leftFront;
  pros::Motor *rightBack;
  pros::Motor *leftBack;

public:
  Drivetrain(pros::Motor *rightFront, pros::Motor *leftFront, pros::Motor *rightBack, pros::Motor *leftBack);
  virtual ~Drivetrain();

  bool is_offset_within(double distance) override;
  void move_for(double right_distance, double left_distance, uint32_t max_rpm = 60, bool block = true);
  void forwards(double distance, uint32_t max_rpm = 100, bool block = true);
  void backwards(double distance, uint32_t max_rpm = 100, bool block = true);
  void turn_right(double angle, uint32_t max_rpm = 100, bool block = true);
  void turn_left(double angle, uint32_t max_rpm = 100, bool block = true);
  void move_right(uint32_t voltage);
  void move_left(uint32_t voltage);

  void update(Controller *controller) override;
  void reset() override;
  void stop() override;
};

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

  unsigned char digitalSpeed = 127;

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

  unsigned char digital_speed() override;
  void digital_speed(unsigned char speed) override;

  void set_line(unsigned char line, unsigned char col, const char *str) override;
  void clear_line(unsigned char line) override;

  void rumble(const char *str) override;

  void update() override;
  void reset() override;
  void stop() override;
};

class Robot : public Resettable {
public:
  Drivetrain *drivetrain;
  Controller *controller;

public:
  explicit Robot(Drivetrain *drivetrain);

  void update();
  void reset() override;
  void stop() override;
  ~Robot();
};

#endif//ROBOT_HPP
