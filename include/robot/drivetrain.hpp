#ifndef ROBOT_DRIVETRAIN_HPP
#define ROBOT_DRIVETRAIN_HPP

#include "robot/controller/controller.hpp"
#include "robot/device/gyro.hpp"
#include "robot/device/inertial.hpp"
#include "robot/device/motor.hpp"
#include <cstdint>

namespace robot {
/**
 * Represents the drivetrain of the robot.
 */
class Drivetrain {
private:
  enum TargetType {
    NONE,

    OPERATOR_DIRECT, // power L/R

    STATIC_TURN, // abs angle
    DIRECT_MOVE, // dist
  };
public:
  device::Motor rightFront;
  device::Motor leftFront;
  device::Motor rightBack;
  device::Motor leftBack;
  device::Inertial imu;

  double kp = 1.0;
  double ki = 0.0;
  double kd = 0.0;

private:
  TargetType targetType = NONE;
  double targetHeading = 0.0;

  int16_t powerLeft = 0;
  int16_t powerRight = 0;

  double targetLeft = 0.0;
  double targetRight = 0.0;

  bool arcade = false;

  double heading = 0.0;
  double rightPos = 0.0;
  double leftPos = 0.0;

  double errorRight = 0.0;
  double errorLeft = 0.0;
  double prevErrorRight = 0.0;
  double prevErrorLeft = 0.0;

  double integralRight = 0.0;
  double integralLeft = 0.0;

  uint16_t timeOff = 0;
  uint16_t timeAtTarget = 0;

public:
  /**
   * Creates a new drivetrain with the specified motors.
   * @param rightFront The motor on the front of the robot and the right side.
   * @param leftFront The motor on the front of the robot and the left side.
   * @param rightBack The motor on the back of the robot and the right side.
   * @param leftBack The motor on the back of the robot and the left side.
   */
  Drivetrain(uint8_t rightFront, uint8_t leftFront, uint8_t rightBack, uint8_t leftBack, uint8_t inertial);
  virtual ~Drivetrain();

  /**
   * Drives the robot forwards by the specified distance.
   * @param distance The distance (in inches) to drive forwards for.
   * @param max_rpm The maximum allowable RPM for the motor to run at while moving.
   * @param block Whether this function should wait for the robot to turn or exit immediately.
   */
  void forwards(double distance, bool wait = true);

  /**
   * Drives the robot backwards by the specified distance.
   * @param distance The distance (in inches) to drive backwards for.
   * @param max_rpm The maximum allowable RPM for the motor to run at while moving.
   * @param block Whether this function should wait for the robot to turn or exit immediately.
   */
  void backwards(double distance, bool wait = true);

  /**
   * Turns the robot to the right by spinning by the specified number of degrees.
   * @param degrees The number of degrees to turn.
   * @param max_rpm The maximum allowable RPM for the motor to run at while turning.
   * @param block Whether this function should wait for the robot to turn or exit immediately.
   */
  void turn_right(double degrees, bool wait = true);

  /**
   * Turns the robot to the left by spinning by the specified number of degrees.
   * @param degrees The number of degrees to turn.
   * @param max_rpm The maximum allowable RPM for the motor to run at while turning.
   * @param block Whether this function should wait for the robot to turn or exit immediately.
   */
  void turn_left(double degrees, bool wait = true);

  void await_move() const;

  /**
   * Checks if the distance between the drivetrain's target position and actual position is within a specific distance.
   * @return Whether the distance between the drivetrain's target position and actual position is in the desired range.
   */
  [[nodiscard]] bool is_at_target() const;

  /**
   * Resets the all of the motors' absolute position trackers.
   */
  void tare();

  /**
   * Engages the brakes of all of the motors.
   */
  void brake();
  void set_brake_mode(pros::motor_brake_mode_e brake_mode);

  void updateTargeting(Controller *controller);
  void updatePosition();
  void updateMovement();

private:
  void setTarget(Drivetrain::TargetType type);

  /**
   * Moves the two right motors of the drivetrain at the specified voltage.
   * @param voltage The voltage to run at [-127 - 127]
   */
  void power_right(int16_t millivolts);

  /**
   * Moves the two left motors of the drivetrain at the specified voltage.
   * @param voltage The voltage to run at [-127 - 127]
   */
  void power_left(int16_t millivolts);

  /**
   * Moves the two right motors of the drivetrain by the specified distance.
   * @param target The distance in ENCODER UNITS to move the right motors by.
   */
  void move_right_targeting(double target);

  /**
   * Moves the two left motors of the drivetrain by the specified distance.
   * @param target The distance in ENCODER UNITS to move the right motors by.
   */
  void move_left_targeting(double target);
};
} // namespace robot
#endif // ROBOT_DRIVETRAIN_HPP
