#ifndef ROBOT_AUTONOMOUS_RIGHT_WINPOINT_HPP
#define ROBOT_AUTONOMOUS_RIGHT_WINPOINT_HPP
#include "robot/autonomous/autonomous.hpp"

namespace robot::autonomous {
class RightWinpoint : public Autonomous {
private:
public:
  void update(Robot *robot) override;
};
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_RIGHT_WINPOINT_HPP