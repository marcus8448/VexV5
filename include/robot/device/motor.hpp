#ifndef VEXV5_ROBOT_DEVICE_MOTOR_HPP
#define VEXV5_ROBOT_DEVICE_MOTOR_HPP

#include "device.hpp"
#include "pros/motors.h"
#include <cmath>

#define MOTOR_MAX_MILLIVOLTS 12000

#define DEFAULT_MOTOR_GEARSET pros::E_MOTOR_GEARSET_18
#define DEFAULT_MOTOR_BRAKE pros::E_MOTOR_BRAKE_BRAKE
#define MOTOR_ENCODER_UNITS pros::E_MOTOR_ENCODER_DEGREES

namespace robot::device {
class PID {
public:
  double kp;
  double ki;
  double kd;
  double integralRange;
  double acceptableError;

private:
  double error = 0.0;
  double prevError = 0.0;
  double integral = 0.0;

public:
  explicit PID(double Kp, double Ki, double Kd, double integralRange, double acceptableError);

  void resetState();

  [[nodiscard]] double getError() const;

  double update(double target, double value);
};

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
  explicit Motor(uint8_t port, const char *name, bool reversed = false,
                 pros::motor_gearset_e_t gearset = DEFAULT_MOTOR_GEARSET,
                 pros::motor_brake_mode_e_t brake_mode = DEFAULT_MOTOR_BRAKE);

  void moveVelocity(int16_t velocity);
  void moveMillivolts(int16_t mV);

  void moveAbsolute(double position, int16_t velocity);
  void moveRelative(double amount, int16_t velocity);
  void moveTargetRelative(double amount, int16_t velocity);

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

  void reconfigure() const override;

  void tare();
  void brake();

  [[nodiscard]] Motor::TargetType getTargetType() const;

  static constexpr int16_t gearsetMaxVelocity(pros::motor_gearset_e_t gearset);
};

} // namespace robot::device
#endif // VEXV5_ROBOT_DEVICE_MOTOR_HPP
