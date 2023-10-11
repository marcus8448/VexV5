#ifndef ROBOT_DEVICE_MOTOR_HPP
#define ROBOT_DEVICE_MOTOR_HPP

#include "device.hpp"
#include "pros/motors.h"
#include <array>
#include <numeric>

namespace robot::device {

class Motor {
public:
  static constexpr int16_t MAX_MILLIVOLTS = 12000;
  enum TargetType {
    VOLTAGE,
    VELOCITY
  };

  virtual ~Motor() = default;

  virtual void moveVelocity(int16_t velocity) = 0;
  virtual void moveMillivolts(int16_t mV) = 0;

  virtual void moveAbsolute(double position, int16_t velocity) = 0;
  virtual void moveRelative(double amount, int16_t velocity) = 0;
  virtual void moveTargetRelative(double amount, int16_t velocity) = 0;

  virtual void setBrakeMode(pros::motor_brake_mode_e brake_mode) = 0;

  [[nodiscard]] virtual double getVelocity() const = 0;
  [[nodiscard]] virtual double getEfficiency() const = 0;
  [[nodiscard]] virtual int32_t getTargetVelocity() const = 0;
  [[nodiscard]] virtual int32_t getTargetVoltage() const = 0;

  [[nodiscard]] virtual double getPosition() const = 0;
  [[nodiscard]] virtual double getTargetPosition() const = 0;

  [[nodiscard]] virtual pros::motor_brake_mode_e_t getBrakeMode() const = 0;

  [[nodiscard]] virtual double getTemperature() const = 0;
  [[nodiscard]] virtual double getPower() const = 0;
  [[nodiscard]] virtual double getTorque() const = 0;

  [[nodiscard]] virtual int32_t getCurrentDraw() const = 0;
  [[nodiscard]] virtual int32_t getVoltage() const = 0;

  virtual void tare() = 0;
  virtual void brake() = 0;

  static constexpr int16_t gearsetMaxVelocity(pros::motor_gearset_e_t gearset);
};

class DirectMotor : public Device, public Motor {
private:
  Motor::TargetType targetType = Motor::TargetType::VOLTAGE;
  int16_t target = 0;
  double targetPosition = std::numeric_limits<double>::infinity();

  const pros::motor_gearset_e_t gearset;
  const int16_t maxVelocity;
  pros::motor_brake_mode_e_t brakeMode;

public:
  explicit DirectMotor(int8_t port, const char *name, bool reversed = false,
                 pros::motor_gearset_e_t gearset = pros::E_MOTOR_GEARSET_18,
                 pros::motor_brake_mode_e_t brake_mode = pros::E_MOTOR_BRAKE_BRAKE);
  ~DirectMotor() override = default;

  void moveVelocity(int16_t velocity) override;
  void moveMillivolts(int16_t mV) override;

  void moveAbsolute(double position, int16_t velocity) override;
  void moveRelative(double amount, int16_t velocity) override;
  void moveTargetRelative(double amount, int16_t velocity) override;

  void setBrakeMode(pros::motor_brake_mode_e brake_mode) override;

  [[nodiscard]] double getVelocity() const override;
  [[nodiscard]] double getEfficiency() const override;
  [[nodiscard]] int32_t getTargetVelocity() const override;
  [[nodiscard]] int32_t getTargetVoltage() const override;

  [[nodiscard]] double getPosition() const override;
  [[nodiscard]] double getTargetPosition() const override;

  [[nodiscard]] pros::motor_brake_mode_e_t getBrakeMode() const override;
  [[nodiscard]] pros::motor_gearset_e_t getGearset() const;
  [[nodiscard]] bool isConnected() const override;

  [[nodiscard]] double getTemperature() const override;
  [[nodiscard]] double getPower() const override;
  [[nodiscard]] double getTorque() const override;

  [[nodiscard]] int32_t getCurrentDraw() const override;
  [[nodiscard]] int32_t getVoltage() const override;

  void reconfigure() const override;

  void tare() override;
  void brake() override;
};

template<uint8_t MOTORS> class MotorGroup : public Motor {
private:
  Motor::TargetType targetType = Motor::TargetType::VOLTAGE;
  int16_t target = 0;
  double targetPosition = std::numeric_limits<double>::infinity();

  int16_t maxVelocity;
  pros::motor_brake_mode_e_t brakeMode;

  std::array<int8_t, MOTORS> motors;

public:
  explicit MotorGroup(std::array<int8_t, MOTORS> motors, const char *name, pros::motor_gearset_e_t gearset, pros::motor_brake_mode_e_t brake_mode);
  ~MotorGroup() override = default;

  void moveVelocity(int16_t velocity) override;
  void moveMillivolts(int16_t mV) override;

  void moveAbsolute(double position, int16_t velocity) override;
  void moveRelative(double amount, int16_t velocity) override;
  void moveTargetRelative(double amount, int16_t velocity) override;

  void setBrakeMode(pros::motor_brake_mode_e brake_mode) override;

  [[nodiscard]] double getVelocity() const override;
  [[nodiscard]] double getEfficiency() const override;
  [[nodiscard]] int32_t getTargetVelocity() const override;
  [[nodiscard]] int32_t getTargetVoltage() const override;

  [[nodiscard]] double getPosition() const override;
  [[nodiscard]] double getTargetPosition() const override;

  [[nodiscard]] pros::motor_brake_mode_e_t getBrakeMode() const override;

  [[nodiscard]] double getTemperature() const override;
  [[nodiscard]] double getPower() const override;
  [[nodiscard]] double getTorque() const override;

  [[nodiscard]] int32_t getCurrentDraw() const override;
  [[nodiscard]] int32_t getVoltage() const override;

  void tare() override;
  void brake() override;
};
} // namespace robot::device
#endif // ROBOT_DEVICE_MOTOR_HPP
