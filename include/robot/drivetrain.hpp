#ifndef ROBOT_DRIVETRAIN_HPP
#define ROBOT_DRIVETRAIN_HPP

#include "control/input/controller.hpp"
#include "device/inertial.hpp"
#include "device/motor.hpp"
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
  /**
   * The different control schemes for the drivetrain
   */
  enum ControlScheme {
    /**
     * Left stick y-axis controls the left side.
     * Right stick y-axis controls the right side.
     */
    TANK,

    /**
     * Left stick y-axis determines the overall power.
     * Right stick x-axis determines the balance between the left and right motors.
     */
    ARCADE
  };

  device::Motor rightFront;
  device::Motor leftFront;
  device::Motor rightBack;
  device::Motor leftBack;
  device::Inertial imu;

  double kp = 1.0;
  double ki = 0.0;
  double kd = 0.0;

#ifndef ARCADE_DRIVE
  ControlScheme controlScheme = TANK;
#else
  ControlScheme controlScheme = ARCADE;
#endif

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
   * @param rightFront The port of the motor on the front of the robot and the right side.
   * @param leftFront The port of the motor on the front of the robot and the left side.
   * @param rightBack The port of the motor on the back of the robot and the right side.
   * @param leftBack The port of the motor on the back of the robot and the left side.
   */
  Drivetrain(uint8_t rightFront, uint8_t leftFront, uint8_t rightBack, uint8_t leftBack, uint8_t inertial);
  virtual ~Drivetrain();

  /**
   * Drives the robot forwards by the specified distance.
   * @param distance The distance (in inches) to drive forwards for.
   * @param wait Whether this function should wait for the robot to move or return immediately.
   */
  void forwards(double distance, bool wait = true);

  /**
   * Drives the robot backwards by the specified distance.
   * @param distance The distance (in inches) to drive backwards for.
   * @param block Whether this function should wait for the robot to move or return immediately.
   */
  void backwards(double distance, bool wait = true);

  /**
   * Turns the robot to the right by spinning by the specified number of degrees.
   * @param degrees The number of degrees to turn.
   * @param wait Whether this function should wait for the robot to turn or return immediately.
   */
  void turnRight(double degrees, bool wait = true);

  /**
   * Turns the robot to the left by spinning by the specified number of degrees.
   * @param degrees The number of degrees to turn.
   * @param wait Whether this function should wait for the robot to turn or return immediately.
   */
  void turnLeft(double degrees, bool wait = true);

  /**
   * Blocks until the robot finishes moving.
   */
  void awaitMovement() const;

  /**
   * Checks if the drivetrain has stabilized at its target position.
   * @return Whether the drivetrain has stabilized at its target position.
   */
  [[nodiscard]] bool isAtTarget() const;

  /**
   * Resets the all of the motors' absolute position trackers.
   */
  void tare();

  /**
   * Engages the brakes of all of the motors.
   */
  void brake();
  void setBrakeMode(pros::motor_brake_mode_e brake_mode);

  void updateTargeting(control::input::Controller *controller);
  void updatePosition();
  void updateMovement();

private:
  void setTarget(Drivetrain::TargetType type);

  /**
   * Moves the two right motors of the drivetrain at the specified voltage.
   * @param voltage The voltage to run at [-127 - 127]
   */
  void moveRight(int16_t millivolts);

  /**
   * Moves the two left motors of the drivetrain at the specified voltage.
   * @param voltage The voltage to run at [-127 - 127]
   */
  void moveLeft(int16_t millivolts);

  /**
   * Moves the two right motors of the drivetrain by the specified distance.
   * @param target The distance in ENCODER UNITS to move the right motors by.
   */
  void moveRightTargeting(double target);

  /**
   * Moves the two left motors of the drivetrain by the specified distance.
   * @param target The distance in ENCODER UNITS to move the right motors by.
   */
  void moveLeftTargeting(double target);
};

[[nodiscard]] const char *driveSchemeName(Drivetrain::ControlScheme scheme);
} // namespace robot
#endif // ROBOT_DRIVETRAIN_HPP
