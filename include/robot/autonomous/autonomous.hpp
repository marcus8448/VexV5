#ifndef ROBOT_AUTONOMOUS_AUTONOMOUS_HPP
#define ROBOT_AUTONOMOUS_AUTONOMOUS_HPP

#include "robot/robot.hpp"

namespace robot::autonomous {
/**
 * An autonomous run.
 * Contains all of the code necessary to run the robot for the 15 second autonomous period.
 */
class Autonomous {
public:
  /**
   * Runs the autonomous instance.
   * Blocking call - does not return until the autonomous run is completed. May not be the full 15 seconds.
   * @param robot
   */
  virtual void run(Robot &robot) = 0;
};

/**
 * Registers an autonomous instance to the system.
 * @param name The name of the autonomous run, to be shown on the screen. Must be unique.
 * @param autonomous The autonomous run
 */
void register_autonomous(const char *name, Autonomous *autonomous);
const std::string *get_active();
void set_active(const std::string *program);

/**
 * Returns the selected autonomous run.
 * @return The selected autonomous run.
 */
Autonomous *get_autonomous();
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_AUTONOMOUS_HPP
