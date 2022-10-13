#ifndef ROBOT_AUTONOMOUS_LEFT_SCORE_HPP
#define ROBOT_AUTONOMOUS_LEFT_SCORE_HPP
#include "robot/autonomous/autonomous.hpp"

namespace robot::autonomous {
class LeftScore : public Autonomous {
private:
public:
  void update(Robot *robot) override;
};
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_LEFT_SCORE_HPP