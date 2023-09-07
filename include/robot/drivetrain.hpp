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
    CURVE_MOVE
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

  device::Motor motorL1;
  device::Motor motorL2;
  device::Motor motorL3;
  device::Motor motorR1;
  device::Motor motorR2;
  device::Motor motorR3;

  device::Inertial imu;

  device::PID velRightPID;
  device::PID velLeftPID;

  device::PID rightPID;
  device::PID leftPID;
  device::PID headingPID;

#ifndef USE_ARCADE_DRIVE
  ControlScheme controlScheme = TANK;
#else
  ControlScheme controlScheme = ARCADE;
#endif

  double cPosX = 0.0; // not anchored to anything - rel to start pos
  double cPosY = 0.0;
  double rPosX = +6.25;
  double rPosY = 0.0;
  double lPosX = -6.25;
  double lPosY = 0.0;

private:
  TargetType targetType = NONE;
  double targetHeading = 0.0;

  int16_t powerLeft = 0;
  int16_t powerRight = 0;

  double targetLeft = 0.0;
  double targetRight = 0.0;

  double heading = 0.0;
  double rightPos = 0.0;
  double leftPos = 0.0;

  uint16_t timeOff = 0;
  uint16_t timeAtTarget = 0;

  double targetPosX = 0.0;
  double targetPosY = 0.0;

  double curve = 0.0;
  double endCurveX = 0.0;
  double endCurveY = 0.0;
  double curveAngle = 0.0;

public:
  /**
   * Creates a new drivetrain with the specified motors.
   * @param rightFront The port of the motor on the front of the robot and the right side.
   * @param leftFront The port of the motor on the front of the robot and the left side.
   * @param rightBack The port of the motor on the back of the robot and the right side.
   * @param leftBack The port of the motor on the back of the robot and the left side.
   */
  Drivetrain(int8_t left1, int8_t left2, int8_t left3, int8_t right1, int8_t right2, int8_t right3,
             int8_t inertial);

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

  //  void directMove(double relInFwd, double relInLat);

  void curveTargeting(double relInFwd, double relInLat, double curve, double relAngle);

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
  void updateState();

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
};

[[nodiscard]] const char *driveSchemeName(Drivetrain::ControlScheme scheme);
} // namespace robot
#endif // ROBOT_DRIVETRAIN_HPP
