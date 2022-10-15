#ifndef ROBOT_AUTONOMOUS_RIGHT_SCORE_HPP
#define ROBOT_AUTONOMOUS_RIGHT_SCORE_HPP
#include "robot/autonomous/autonomous.hpp"

namespace robot::autonomous {
class RightScore : public Autonomous {
private:
public:
  void run(Robot *robot) override;
};
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_RIGHT_SCORE_HPP