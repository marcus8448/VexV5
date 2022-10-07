#include "robot/autonomous/autonomous.hpp"
#include <vector>

namespace robot::autonomous {
std::vector<robot::autonomous::Autonomous *> *autonomousPrograms = new std::vector<robot::autonomous::Autonomous *>();
static int32_t program = 0;

void register_autonomous(Autonomous *program) {
    autonomousPrograms->push_back(program);
}

void set_active(int32_t index) {
    program = index;
}

[[noreturn]] void run(Robot *robot) {
    Autonomous *autonomous = autonomousPrograms->at(program);
    while (true) {
        autonomous->update(robot);
    }
}
} // namespace robot::autonomous
