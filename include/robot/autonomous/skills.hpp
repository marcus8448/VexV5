#ifndef ROBOT_AUTONOMOUS_SKILLS_HPP
#define ROBOT_AUTONOMOUS_SKILLS_HPP
#include "robot/autonomous/autonomous.hpp"

namespace robot::autonomous {

/**
 * Autonomous run to be used when aiming for the best score..
 */
class Skills : public Autonomous {
public:
  explicit Skills() : Autonomous("Skills"){};

  void run(Robot &robot) override;
};
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_SKILLS_HPP
