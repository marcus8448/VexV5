#ifndef ROBOT_ROBOT_HPP
#define ROBOT_ROBOT_HPP

#include "controller/controller.hpp"
#include "drivetrain.hpp"
#include "expansion.hpp"
#include "flywheel.hpp"
#include "indexer.hpp"
#include "intake.hpp"

namespace robot {
/**
 * Represents the robot, broken into subsystems.
 */
class Robot {
public:
  /**
   * The robot's drivetrain.
   */
  Drivetrain *drivetrain;
  /**
   * The robot's intake.
   */
  Intake *intake;
  /**
   * The robot's indexer.
   */
  Indexer *indexer;
  /**
   * The robot's flywheel.
   */
  Flywheel *flywheel;
  /**
   * The robot's expansion.
   */
  Expansion *expansion;
  /**
   * The robot's controller. Used during operator control.
   */
  Controller *controller;

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
  explicit Robot(Drivetrain *drivetrain, Intake *intake, Indexer *indexer, Flywheel *flywheel, Expansion *expansion);
  explicit Robot(const Robot &robot) = delete;
  ~Robot();

  /**
   * Called every 20ms during operator control.
   */
  void update() const;
};
} // namespace robot

#endif // ROBOT_ROBOT_HPP
