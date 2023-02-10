#ifndef ROBOT_AUTONOMOUS_LEFT_SCORE_HPP
#define ROBOT_AUTONOMOUS_LEFT_SCORE_HPP
#include "robot/autonomous/autonomous.hpp"

namespace robot::autonomous {

/**
 * Autonomous run to be used when aiming for the best score when starting on the left side.
 */
class LeftSkills : public Autonomous {
public:
  explicit LeftSkills() : Autonomous("Left Skills"){};

  void run(AutonomousContext &context) override;
};
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_LEFT_SCORE_HPP
