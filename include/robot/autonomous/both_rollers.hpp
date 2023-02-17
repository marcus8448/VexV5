#ifndef ROBOT_AUTONOMOUS_BOTH_ROLLERS_HPP
#define ROBOT_AUTONOMOUS_BOTH_ROLLERS_HPP
#include "robot/autonomous/autonomous.hpp"

namespace robot::autonomous {

class BothRollers : public Autonomous {
public:
  explicit BothRollers() : Autonomous("Both Rollers"){};

  void run(AutonomousContext &context) override;
};
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_BOTH_ROLLERS_HPP
