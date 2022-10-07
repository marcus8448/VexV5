#ifndef ROBOT_AUTONOMOUS_AUTONOMOUS_HPP
#define ROBOT_AUTONOMOUS_AUTONOMOUS_HPP

#include "robot/robot.hpp"

namespace robot::autonomous {
class Autonomous {
    public:
    virtual void update(Robot *robot);
    virtual const char *name();
};

void register_autonomous(Autonomous *autonomous);
void set_active(int32_t index);

[[noreturn]] void run(Robot *robot);
} // namespace robot::autonomous
#endif // ROBOT_AUTONOMOUS_AUTONOMOUS_HPP