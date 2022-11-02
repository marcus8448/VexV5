#ifndef VEXV5_ROBOT_DEVICE_MOTOR_HPP
#define VEXV5_ROBOT_DEVICE_MOTOR_HPP

#include "pros/motors.hpp"
#include <cmath>

#define MOTOR_TIMEOUT_MILLIS 4000

namespace robot::device {
class Motor {
  enum TargetType { VOLTAGE, VELOCITY };

private:
  pros::Motor motor;

  TargetType targetType = TargetType::VOLTAGE;
  int32_t target = 0;
  double targetPosition = INFINITY;
  double prev_target = INFINITY;

  const pros::motor_gearset_e_t gearset;
  const int32_t maxVelocity;
  pros::motor_brake_mode_e_t brakeMode;
  bool reversed;

  const uint8_t port;

public:
  explicit Motor(pros::Motor motor);
  explicit Motor(uint8_t port, pros::motor_gearset_e_t gearset = pros::E_MOTOR_GEARSET_18,
                 pros::motor_brake_mode_e_t brake_mode = pros::E_MOTOR_BRAKE_BRAKE, bool reversed = false);
  explicit Motor(uint8_t port, bool reversed = false);

  ~Motor();

  void move_velocity(int32_t target_velocity);
  void move_millivolts(int16_t target_voltage);
  void move_percentage(double percent);

  void move_absolute(double target_position, int32_t target_velocity);
  void move_relative(double target_position, int32_t target_velocity);
  void move_relative_target(double target_position, int32_t target_velocity);

  [[nodiscard]] bool is_at_velocity(int32_t target_velocity) const;
  void await_velocity(int32_t target_velocity, int16_t timeout_millis = MOTOR_TIMEOUT_MILLIS) const;

  [[nodiscard]] double get_velocity() const;
  [[nodiscard]] double get_efficiency() const;
  [[nodiscard]] int32_t get_target_velocity() const;
  [[nodiscard]] int32_t get_target_voltage() const;

  [[nodiscard]] double get_position() const;
  [[nodiscard]] double get_target_position() const;

  [[nodiscard]] pros::motor_brake_mode_e_t get_brake_mode() const;
  [[nodiscard]] pros::motor_gearset_e_t get_gearset() const;
  [[nodiscard]] bool is_reversed() const;
  [[nodiscard]] uint8_t get_port() const;

  [[nodiscard]] double get_temperature() const;

  void await_target(int16_t timeout_millis = MOTOR_TIMEOUT_MILLIS) const;
  [[nodiscard]] bool is_at_target() const;

  void tare();
  void stop();

  [[nodiscard]] Motor::TargetType get_target_type() const;
  [[nodiscard]] pros::Motor get_raw_motor() const;
};

int32_t get_gearset_max_velocity(pros::motor_gearset_e_t gearset);
} // namespace robot::device
#endif // VEXV5_ROBOT_DEVICE_MOTOR_HPP
