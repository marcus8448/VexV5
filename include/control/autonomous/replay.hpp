#ifndef CONTROL_AUTONOMOUS_REPLAY_HPP
#define CONTROL_AUTONOMOUS_REPLAY_HPP
#include "autonomous.hpp"

namespace control::autonomous {

class Replay : public Autonomous {
public:
  explicit Replay(const char *name);

  void run(Robot &robot) override;
};
} // namespace control::autonomous
#endif // CONTROL_AUTONOMOUS_REPLAY_HPP
