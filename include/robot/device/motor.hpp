#ifndef VEXV5_ROBOT_DEVICE_MOTOR_HPP
#define VEXV5_ROBOT_DEVICE_MOTOR_HPP

#include "pros/motors.hpp"

namespace robot::device {
enum TargetType {
  VOLTAGE,
  VELOCITY
};

class Motor {
private:
  pros::Motor motor;

  TargetType targetType = TargetType::VOLTAGE;
  double targetVelocity = 0.0;
  int32_t targetVoltage = 0;

  const pros::motor_gearset_e_t gearSet;
  pros::motor_brake_mode_e_t brakeMode;
  bool reversed;

  const uint8_t port;
public:
  explicit Motor(pros::Motor motor);
  explicit Motor(uint8_t port, pros::motor_gearset_e_t gearset = pros::E_MOTOR_GEARSET_18, pros::motor_brake_mode_e_t brake_mode = pros::E_MOTOR_BRAKE_BRAKE, bool reversed = false);
  explicit Motor(uint8_t port, bool reversed = false);

  ~Motor();

  void move_velocity(double target_velocity);
  void move_voltage(int32_t target_voltage);

  void is_at_velocity(bool block = false);
  void await_velocity(double target_velocity);

  [[nodiscard]] bool is_offset_within(double distance) const;

  void tare();
  void stop();

  [[nodiscard]] TargetType get_target_type() const;
  pros::Motor get_raw_motor();
};
} // namespace robot::device
#endif // VEXV5_ROBOT_DEVICE_MOTOR_HPP
