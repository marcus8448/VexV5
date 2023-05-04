#ifndef VEXV5_ROBOT_DEVICE_MOTOR_HPP
#define VEXV5_ROBOT_DEVICE_MOTOR_HPP

#include "include/debug/logger.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "robot/device/device.hpp"
#include "robot/device/motor.hpp"
#include "robot/pid/pid_controller.hpp"
#include "robot/pid/vex_pid.hpp"
#include "robot/updatable.hpp"
#include <cerrno>
#include <cmath>

#define MOTOR_TIMEOUT_MILLIS 4000
#define MOTOR_MAX_MILLIVOLTS 12000

namespace robot::device {
class Motor : public Device {
private:
  enum TargetType { VOLTAGE, VELOCITY };

  TargetType targetType = TargetType::VOLTAGE;
  int16_t target = 0;
  double targetPosition = INFINITY;

  const pros::motor_gearset_e_t gearset;
  const int16_t maxVelocity;
  pros::motor_brake_mode_e_t brakeMode;
  bool reversed;

public:
  PidController *controller = nullptr;
  DEVICE_TYPE_NAME("Motor")

  explicit Motor(uint8_t port, const char *name, pros::motor_gearset_e_t gearset = pros::E_MOTOR_GEARSET_18,
                 pros::motor_brake_mode_e_t brake_mode = pros::E_MOTOR_BRAKE_BRAKE, bool reversed = false);
  explicit Motor(uint8_t port, const char *name, bool reversed = false);

  ~Motor() override;

  void move_velocity(int16_t velocity);
  void move_millivolts(int16_t mV);
  void move_percentage(double percent);

  void move_absolute(double position, int16_t velocity);
  void move_relative(double amount, int16_t velocity);
  void move_target_relative(double amount, int16_t velocity);

  void set_reversed(bool reverse);
  void set_brake_mode(pros::motor_brake_mode_e brake_mode);

  [[nodiscard]] double get_velocity() const;
  [[nodiscard]] double get_efficiency() const;
  [[nodiscard]] int32_t get_target_velocity() const;
  [[nodiscard]] int32_t get_target_voltage() const;

  [[nodiscard]] double get_position() const;
  [[nodiscard]] double get_target_position() const;

  [[nodiscard]] pros::motor_brake_mode_e_t get_brake_mode() const;
  [[nodiscard]] pros::motor_gearset_e_t get_gearset() const;
  [[nodiscard]] bool is_reversed() const;
  [[nodiscard]] bool is_connected() const override;

  [[nodiscard]] double get_temperature() const;
  [[nodiscard]] double get_power() const;
  [[nodiscard]] double get_torque() const;

  [[nodiscard]] int32_t get_current_draw() const;
  [[nodiscard]] int32_t get_voltage() const;
  [[nodiscard]] int32_t get_current_limit() const;
  [[nodiscard]] int32_t get_voltage_limit() const;

  void await_target(int16_t timeout_millis = MOTOR_TIMEOUT_MILLIS) const;
  [[nodiscard]] bool is_at_target() const;

  void update() override;
  void reconfigure() const override;

  void tare();
  void brake();

  [[nodiscard]] Motor::TargetType get_target_type() const;
};

int16_t get_gearset_max_velocity(pros::motor_gearset_e_t gearset);
} // namespace robot::device
#endif // VEXV5_ROBOT_DEVICE_MOTOR_HPP
