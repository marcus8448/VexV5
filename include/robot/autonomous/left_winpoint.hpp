#ifndef ROBOT_AUTONOMOUS_LEFT_WINPOINT_HPP
#define ROBOT_AUTONOMOUS_LEFT_WINPOINT_HPP
#include "robot/autonomous/autonomous.hpp"

namespace robot::autonomous {
class LeftWinpoint : public Autonomous {
private:
public:
  void update(Robot *robot) override;
  const char *name() override;
};
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_LEFT_WINPOINT_HPP