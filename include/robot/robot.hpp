#ifndef ROBOT_ROBOT_HPP
#define ROBOT_ROBOT_HPP

#include "arm.hpp"
#include "control/input/controller.hpp"
#include "drivetrain.hpp"
#include "intake.hpp"
#include <string>

namespace robot {
/**
 * Represents the robot, broken into subsystems.
 */
class Robot {
public:
  /**
   * The robot's drivetrain.
   */
  Drivetrain drivetrain;
  /**
   * The robot's arm.
   */
  Arm arm;
  /**
   * The robot's intake
   */
  Intake intake;
  /**
   * The robot's controller. Used during operator control.
   */
  control::input::Controller *controller;

  std::string autonomous = "None";

public:
  /**
   * Creates a new robot with the specified components.
   * @param drivetrain The robot's drivetrain.
   * @param intake The robot's intake.
   * @param flywheel The robot's flywheel.
   * @param expansion The robot's expansion.
   * @param indexer The robot's indexer.
   * @param roller The robot's roller.
   */
  explicit Robot(uint8_t driveL1, uint8_t driveL2, uint8_t driveL3, uint8_t driveR1, uint8_t driveR2, uint8_t driveR3,
                 uint8_t intakeRight, uint8_t intakeLeft, uint8_t inertial, uint8_t arm1, uint8_t arm2);
  explicit Robot(const Robot &robot) = delete;
  ~Robot();

  void setController(control::input::Controller *controller);

  void updateDevices();

  void runAutonomous();
  [[noreturn]] void opcontrol();
};
} // namespace robot

#endif // ROBOT_ROBOT_HPP
