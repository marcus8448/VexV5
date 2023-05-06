#ifndef CONTROL_AUTONOMOUS_LEFT_WINPOINT_HPP
#define CONTROL_AUTONOMOUS_LEFT_WINPOINT_HPP
#include "autonomous.hpp"

namespace robot::autonomous {

/**
 * Autonomous run to be used when trying to earn the autonomous win point when starting on the left side.
 */
class LeftWinpoint : public Autonomous {
public:
  explicit LeftWinpoint() : Autonomous("Left Winpoint"){};

  void run(Robot &robot) override;
};
} // namespace robot::autonomous
#endif // CONTROL_AUTONOMOUS_LEFT_WINPOINT_HPP
