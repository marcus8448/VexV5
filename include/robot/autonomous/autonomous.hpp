#ifndef ROBOT_AUTONOMOUS_AUTONOMOUS_HPP
#define ROBOT_AUTONOMOUS_AUTONOMOUS_HPP

#include "robot/robot.hpp"

namespace robot::autonomous {
class Autonomous {
public:
  virtual void update(Robot *robot) = 0;
  virtual const char *name() = 0;
};

void register_autonomous(Autonomous *autonomous);
void set_active(int32_t index);

[[noreturn]] void run(Robot *robot);
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_AUTONOMOUS_HPP