#ifndef ROBOT_AUTONOMOUS_NONE_HPP
#define ROBOT_AUTONOMOUS_NONE_HPP
#include "robot/autonomous/autonomous.hpp"

namespace robot::autonomous {

/**
 * Autonomous run that does noting.
 */
class None : public Autonomous {
private:
public:
  void run(Robot *robot) override;
};
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_NONE_HPP
