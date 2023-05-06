#ifndef CONTROL_AUTONOMOUS_LEFT_WINPOINT_HPP
#define CONTROL_AUTONOMOUS_LEFT_WINPOINT_HPP
#include "autonomous.hpp"

namespace control::autonomous {

/**
 * Autonomous run to be used when trying to earn the autonomous win point when starting on the left side.
 */
class LeftWinpoint : public Autonomous {
public:
  explicit LeftWinpoint() : Autonomous("Left Winpoint"){};

  void run(Robot &robot) override;
};
} // namespace control::autonomous
#endif // CONTROL_AUTONOMOUS_LEFT_WINPOINT_HPP
