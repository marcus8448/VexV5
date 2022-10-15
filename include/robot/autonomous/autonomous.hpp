#ifndef ROBOT_AUTONOMOUS_AUTONOMOUS_HPP
#define ROBOT_AUTONOMOUS_AUTONOMOUS_HPP

#include "robot/robot.hpp"

namespace robot::autonomous {
class Autonomous {
public:
  virtual void run(Robot *robot) = 0;
};

void register_autonomous(const char *name, Autonomous *autonomous);
void set_active(const char *program);

Autonomous *get_autonomous();
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_AUTONOMOUS_HPP