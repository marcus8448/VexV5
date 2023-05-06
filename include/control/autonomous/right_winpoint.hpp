#ifndef CONTROL_AUTONOMOUS_RIGHT_WINPOINT_HPP
#define CONTROL_AUTONOMOUS_RIGHT_WINPOINT_HPP
#include "autonomous.hpp"

namespace control::autonomous {

/**
 * Autonomous run to be used when trying to earn the autonomous win point when starting on the right side.
 */
class RightWinpoint : public Autonomous {
public:
  explicit RightWinpoint() : Autonomous("Right Winpoint"){};

  void run(Robot &robot) override;
};
} // namespace control::autonomous
#endif // CONTROL_AUTONOMOUS_RIGHT_WINPOINT_HPP
