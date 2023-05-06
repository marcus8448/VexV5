#ifndef CONTROL_AUTONOMOUS_SKILLS_HPP
#define CONTROL_AUTONOMOUS_SKILLS_HPP
#include "autonomous.hpp"

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
#endif // CONTROL_AUTONOMOUS_SKILLS_HPP
