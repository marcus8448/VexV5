#ifndef ROBOT_ROBOT_HPP
#define ROBOT_ROBOT_HPP

#include "control/input/controller.hpp"
#include "drivetrain.hpp"
#include "intake.hpp"
#include "wings.hpp"
#include <memory>
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
//  /**
//   * The robot's intake
//   */
//  Intake intake;
  /**
   * The robot's wings
   */
  Wings wings;
  /**
   * The robot's controller. Used during operator control.
   */
  std::unique_ptr<control::input::Controller> controller;

  std::string autonomous = "None";

  /**
   * Creates a new robot with the specified components.
   * @param drivetrain The robot's drivetrain.
   * @param intake The robot's intake.
   * @param flywheel The robot's flywheel.
   * @param expansion The robot's expansion.
   * @param indexer The robot's indexer.
   * @param roller The robot's roller.
   */
  explicit Robot(int8_t driveL1, int8_t driveL2, int8_t driveL3, int8_t driveR1, int8_t driveR2, int8_t driveR3,
                 int8_t wingsL, int8_t wingsR, int8_t inertial);
  Robot(const Robot &robot) = delete;
  Robot(Robot &&robot) = delete;
  Robot &operator=(const Robot &) = delete;
  Robot &operator=(Robot &&) = delete;

  ~Robot();

  void setController(control::input::Controller *controller);

  void updateDevices();

  void runAutonomous();
  [[noreturn]] void opcontrol();
};
} // namespace robot

#endif // ROBOT_ROBOT_HPP
