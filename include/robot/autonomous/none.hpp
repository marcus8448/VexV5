#ifndef ROBOT_AUTONOMOUS_NONE_HPP
#define ROBOT_AUTONOMOUS_NONE_HPP
#include "robot/autonomous/autonomous.hpp"

namespace robot::autonomous {

/**
 * Autonomous run that does noting.
 */
class None : public Autonomous {
public:
  explicit None() : Autonomous("None") {}

  void run(AutonomousContext &context) override;
};
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_NONE_HPP
