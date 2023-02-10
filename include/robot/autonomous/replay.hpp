#ifndef ROBOT_AUTONOMOUS_REPLAY_HPP
#define ROBOT_AUTONOMOUS_REPLAY_HPP
#include "robot/autonomous/autonomous.hpp"

namespace robot::autonomous {

class Replay : public Autonomous {
public:
  explicit Replay(const char *name);

  void run(AutonomousContext &context) override;
};
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_REPLAY_HPP
