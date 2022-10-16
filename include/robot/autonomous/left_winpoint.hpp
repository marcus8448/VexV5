#ifndef ROBOT_AUTONOMOUS_LEFT_WINPOINT_HPP
#define ROBOT_AUTONOMOUS_LEFT_WINPOINT_HPP
#include "robot/autonomous/autonomous.hpp"

namespace robot::autonomous {

/**
 * Autonomous run to be used when trying to earn the autonomous win point when starting on the left side.
 */
class LeftWinpoint : public Autonomous {
private:
public:
  void run(Robot *robot) override;
};
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_LEFT_WINPOINT_HPP
