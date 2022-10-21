#ifndef ROBOT_DRIVETRAIN_HPP
#define ROBOT_DRIVETRAIN_HPP

#include "pros/motors.hpp"
#include "robot/controller/controller.hpp"
#include <cstdint>

#define DRIVETRAIN_MAX_RPM 100

namespace robot {
/**
 * Represents the drivetrain of the robot.
 */
class Drivetrain : public Updatable {
public:
  pros::Motor *rightFront;
  pros::Motor *leftFront;
  pros::Motor *rightBack;
  pros::Motor *leftBack;

public:
  /**
   * Creates a new drivetrain with the specified motors.
   * @param rightFront The motor on the front of the robot and the right side.
   * @param leftFront The motor on the front of the robot and the left side.
   * @param rightBack The motor on the back of the robot and the right side.
   * @param leftBack The motor on the back of the robot and the left side.
   */
  Drivetrain(pros::Motor *rightFront, pros::Motor *leftFront, pros::Motor *rightBack, pros::Motor *leftBack);
  virtual ~Drivetrain();

  /**
   * Drives the robot forwards by the specified distance.
   * @param distance The distance (in inches) to drive forwards for.
   * @param max_rpm The maximum allowable RPM for the motor to run at while moving.
   * @param block Whether this function should wait for the robot to turn or exit immediately.
   */
  void forwards(double distance, int32_t max_rpm = DRIVETRAIN_MAX_RPM, bool block = true);

  /**
   * Drives the robot backwards by the specified distance.
   * @param distance The distance (in inches) to drive backwards for.
   * @param max_rpm The maximum allowable RPM for the motor to run at while moving.
   * @param block Whether this function should wait for the robot to turn or exit immediately.
   */
  void backwards(double distance, int32_t max_rpm = DRIVETRAIN_MAX_RPM, bool block = true);

  /**
   * Turns the robot to the right by spinning by the specified number of degrees.
   * @param degrees The number of degrees to turn.
   * @param max_rpm The maximum allowable RPM for the motor to run at while turning.
   * @param block Whether this function should wait for the robot to turn or exit immediately.
   */
  void turn_right(double degrees, int32_t max_rpm = DRIVETRAIN_MAX_RPM, bool block = true);

  /**
   * Turns the robot to the left by spinning by the specified number of degrees.
   * @param degrees The number of degrees to turn.
   * @param max_rpm The maximum allowable RPM for the motor to run at while turning.
   * @param block Whether this function should wait for the robot to turn or exit immediately.
   */
  void turn_left(double degrees, int32_t max_rpm = DRIVETRAIN_MAX_RPM, bool block = true);

  /**
   * Moves the two right motors of the drivetrain at the specified voltage.
   * @param voltage The voltage to run at [-127 - 127]
   */
  void move_right(int32_t voltage) const;

  /**
   * Moves the two left motors of the drivetrain at the specified voltage.
   * @param voltage The voltage to run at [-127 - 127]
   */
  void move_left(int32_t voltage) const;

  /**
   * Checks if the distance between the drivetrain's target position and actual position is within a specific distance.
   * @param distance The maximum allowable offset from the target position.
   * @return Whether the distance between the drivetrain's target position and actual position is in the desired range.
   */
  [[nodiscard]] bool is_offset_within(double distance) const;

  /**
   * Resets the all of the motors' absolute position trackers.
   * Also resets the target position of the robot.
   */
  void tare() const;

  /**
   * Engages the brakes of all of the motors.
   */
  void stop() const;

  void update(Controller *controller) override;

private:
  /**
   * Moves the robot by the specified distances.
   * @param right_distance The distance in ENCODER UNITS to move the right motors by.
   * @param left_distance The distance in ENCODER UNITS to move the left motors by.
   * @param max_rpm The maximum allowable RPM for the motor to run at while moving.
   * @param block Whether this function should wait for the robot to move or exit immediately.
   */
  void move(double right_distance, double left_distance, int32_t max_rpm = DRIVETRAIN_MAX_RPM, bool block = true) const;

  /**
   * Moves the two right motors of the drivetrain by the specified distance.
   * @param distance The distance in ENCODER UNITS to move the right motors by.
   */
  void move_right(double distance, int32_t max_rpm = DRIVETRAIN_MAX_RPM) const;

  /**
   * Moves the two left motors of the drivetrain by the specified distance.
   * @param distance The distance in ENCODER UNITS to move the right motors by.
   */
  void move_left(double distance, int32_t max_rpm = DRIVETRAIN_MAX_RPM) const;
};
} // namespace robot
#endif // ROBOT_DRIVETRAIN_HPP
