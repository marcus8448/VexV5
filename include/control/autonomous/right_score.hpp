#ifndef CONTROL_AUTONOMOUS_RIGHT_SCORE_HPP
#define CONTROL_AUTONOMOUS_RIGHT_SCORE_HPP
#include "autonomous.hpp"

namespace control::autonomous {

/**
 * Autonomous run to be used when trying to earn the autonomous win point when starting on the right side.
 */
class RightScore : public Autonomous {
public:
  explicit RightScore() : Autonomous("Right Score"){};

  void run(Robot &robot) override;
};
} // namespace control::autonomous
#endif // CONTROL_AUTONOMOUS_RIGHT_SCORE_HPP
