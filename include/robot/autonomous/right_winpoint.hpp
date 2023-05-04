#ifndef ROBOT_AUTONOMOUS_RIGHT_WINPOINT_HPP
#define ROBOT_AUTONOMOUS_RIGHT_WINPOINT_HPP
#include "robot/autonomous/autonomous.hpp"

namespace robot::autonomous {

/**
 * Autonomous run to be used when trying to earn the autonomous win point when starting on the right side.
 */
class RightWinpoint : public Autonomous {
public:
  explicit RightWinpoint() : Autonomous("Right Winpoint"){};

  void run(Robot &robot) override;
};
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_RIGHT_WINPOINT_HPP
