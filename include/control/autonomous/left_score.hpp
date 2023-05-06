#ifndef CONTROL_AUTONOMOUS_LEFT_SCORE_HPP
#define CONTROL_AUTONOMOUS_LEFT_SCORE_HPP
#include "autonomous.hpp"

namespace robot::autonomous {

/**
 * Autonomous run to be used when trying to earn the autonomous win point when starting on the left side.
 */
class LeftScore : public Autonomous {
public:
  explicit LeftScore() : Autonomous("Left Score"){};

  void run(Robot &robot) override;
};
} // namespace robot::autonomous
#endif // CONTROL_AUTONOMOUS_LEFT_SCORE_HPP
