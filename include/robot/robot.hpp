#ifndef ROBOT_ROBOT_HPP
#define ROBOT_ROBOT_HPP

#include "control/input/controller.hpp"
#include "drivetrain.hpp"

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
   * The robot's controller. Used during operator control.
   */
  control::input::Controller *controller;

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
  explicit Robot(uint8_t rightFront, uint8_t leftFront, uint8_t rightBack, uint8_t leftBack, uint8_t inertial);
  explicit Robot(const Robot &robot) = delete;
  ~Robot();

  void setController(control::input::Controller *controller);

  [[noreturn]] void backgroundControl();
  void runAutonomous();
  [[noreturn]] void opcontrol();
};
} // namespace robot

#endif // ROBOT_ROBOT_HPP
