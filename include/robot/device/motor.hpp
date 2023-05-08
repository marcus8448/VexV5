#ifndef VEXV5_ROBOT_DEVICE_MOTOR_HPP
#define VEXV5_ROBOT_DEVICE_MOTOR_HPP

#include "include/debug/logger.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "robot/device/device.hpp"
#include "robot/device/motor.hpp"
#include "robot/pid/pid.hpp"
#include "robot/pid/pid_controller.hpp"
#include "robot/updatable.hpp"
#include <cerrno>
#include <cmath>

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
  VelocityPid *controller = nullptr;

  explicit Motor(uint8_t port, const char *name, pros::motor_gearset_e_t gearset = pros::E_MOTOR_GEARSET_18,
                 pros::motor_brake_mode_e_t brake_mode = pros::E_MOTOR_BRAKE_BRAKE, bool reversed = false);
  explicit Motor(uint8_t port, const char *name, bool reversed = false);

  ~Motor() override;

  void moveVelocity(int16_t velocity);
  void moveMillivolts(int16_t mV);

  void moveAbsolute(double position, int16_t velocity);
  void moveRelative(double amount, int16_t velocity);
  void moveTargetRelative(double amount, int16_t velocity);

  void setReversed(bool reverse);
  void setBrakeMode(pros::motor_brake_mode_e brake_mode);

  [[nodiscard]] double getVelocity() const;
  [[nodiscard]] double getEfficiency() const;
  [[nodiscard]] int32_t getTargetVelocity() const;
  [[nodiscard]] int32_t getTargetVoltage() const;

  [[nodiscard]] double getPosition() const;
  [[nodiscard]] double getTargetPosition() const;

  [[nodiscard]] pros::motor_brake_mode_e_t getBrakeMode() const;
  [[nodiscard]] pros::motor_gearset_e_t getGearset() const;
  [[nodiscard]] bool isReversed() const;
  [[nodiscard]] bool isConnected() const override;

  [[nodiscard]] double getTemperature() const;
  [[nodiscard]] double getPower() const;
  [[nodiscard]] double getTorque() const;

  [[nodiscard]] int32_t getCurrentDraw() const;
  [[nodiscard]] int32_t getVoltage() const;
  [[nodiscard]] int32_t getCurrentLimit() const;
  [[nodiscard]] int32_t getVoltageLimit() const;

  void update() override;
  void reconfigure() const override;

  void tare();
  void brake();

  [[nodiscard]] Motor::TargetType getTargetType() const;
};

int16_t gearsetMaxVelocity(pros::motor_gearset_e_t gearset);
} // namespace robot::device
#endif // VEXV5_ROBOT_DEVICE_MOTOR_HPP
