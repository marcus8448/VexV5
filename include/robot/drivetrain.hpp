#ifndef ROBOT_DRIVETRAIN_HPP
#define ROBOT_DRIVETRAIN_HPP

#include "robot/controller/controller.hpp"
#include "robot/device/gyro.hpp"
#include "robot/device/inertial.hpp"
#include "robot/device/motor.hpp"
#include <cstdint>

#define DRIVETRAIN_DEFAULT_RPM 80
#define DRIVETRAIN_DEFAULT_TURN_RPM 50
#define DRIVETRAIN_DEFAULT_TIMEOUT 5000

namespace robot {
/**
 * Represents the drivetrain of the robot.
 */
class Drivetrain : public Controlled {
public:
  device::Motor rightFront;
  device::Motor leftFront;
  device::Motor rightBack;
  device::Motor leftBack;
  device::Inertial imu;

  void power(double percent);

private:
  bool arcade = false;
  double targetRotation = 0.0;
  uint16_t timeOff = 0;

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
  void forwards(double distance, int16_t max_rpm = DRIVETRAIN_DEFAULT_RPM,
                uint16_t timeout_millis = DRIVETRAIN_DEFAULT_TIMEOUT);

  /**
   * Drives the robot backwards by the specified distance.
   * @param distance The distance (in inches) to drive backwards for.
   * @param max_rpm The maximum allowable RPM for the motor to run at while moving.
   * @param block Whether this function should wait for the robot to turn or exit immediately.
   */
  void backwards(double distance, int16_t max_rpm = DRIVETRAIN_DEFAULT_RPM,
                 uint16_t timeout_millis = DRIVETRAIN_DEFAULT_TIMEOUT);

  /**
   * Turns the robot to the right by spinning by the specified number of degrees.
   * @param degrees The number of degrees to turn.
   * @param max_rpm The maximum allowable RPM for the motor to run at while turning.
   * @param block Whether this function should wait for the robot to turn or exit immediately.
   */
  void turn_right(double degrees, int16_t max_rpm = DRIVETRAIN_DEFAULT_TURN_RPM,
                  uint16_t timeout_millis = DRIVETRAIN_DEFAULT_TIMEOUT);

  /**
   * Turns the robot to the left by spinning by the specified number of degrees.
   * @param degrees The number of degrees to turn.
   * @param max_rpm The maximum allowable RPM for the motor to run at while turning.
   * @param block Whether this function should wait for the robot to turn or exit immediately.
   */
  void turn_left(double degrees, int16_t max_rpm = DRIVETRAIN_DEFAULT_TURN_RPM,
                 uint16_t timeout_millis = DRIVETRAIN_DEFAULT_TIMEOUT);

  void await_move(uint16_t timeout_millis = DRIVETRAIN_DEFAULT_TIMEOUT) const;

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

  void update(Controller *controller) override;

private:
  /**
   * Moves the robot by the specified distances.
   * @param right_distance The distance in ENCODER UNITS to move the right motors by.
   * @param left_distance The distance in ENCODER UNITS to move the left motors by.
   * @param max_rpm The maximum allowable RPM for the motor to run at while moving.
   * @param block Whether this function should wait for the robot to move or exit immediately.
   */
  void move(double right_distance, double left_distance, int16_t max_rpm = DRIVETRAIN_DEFAULT_RPM);

  /**
   * Moves the two right motors of the drivetrain at the specified voltage.
   * @param voltage The voltage to run at [-127 - 127]
   */
  void power_right(double percent);

  /**
   * Moves the two left motors of the drivetrain at the specified voltage.
   * @param voltage The voltage to run at [-127 - 127]
   */
  void power_left(double percent);

  /**
   * Moves the two right motors of the drivetrain by the specified distance.
   * @param distance The distance in ENCODER UNITS to move the right motors by.
   */
  void move_right_distance(double distance, int16_t max_rpm);

  /**
   * Moves the two left motors of the drivetrain by the specified distance.
   * @param distance The distance in ENCODER UNITS to move the right motors by.
   */
  void move_left_distance(double distance, int16_t max_rpm);
};
} // namespace robot
#endif // ROBOT_DRIVETRAIN_HPP
