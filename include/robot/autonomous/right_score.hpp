#ifndef ROBOT_AUTONOMOUS_RIGHT_SCORE_HPP
#define ROBOT_AUTONOMOUS_RIGHT_SCORE_HPP
#include "robot/autonomous/autonomous.hpp"

namespace robot::autonomous {

/**
 * Autonomous run to be used when aiming for the best score when starting on the right side.
 */
class RightScore : public Autonomous {
private:
public:
  void run(Robot *robot) override;
};
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_RIGHT_SCORE_HPP