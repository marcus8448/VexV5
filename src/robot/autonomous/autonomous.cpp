#include "robot/autonomous/autonomous.hpp"
#include <map>

std::map<const char *, robot::autonomous::Autonomous *> *autonomousPrograms = new std::map<const char *, robot::autonomous::Autonomous *>();
namespace robot::autonomous {
static const char *program = 0;

void register_autonomous(const char* name, Autonomous *program) { (*autonomousPrograms)[name] = program; }

void set_active(const char *program) { robot::autonomous::program = program; }

[[noreturn]] void run(Robot *robot) {
  Autonomous *autonomous = (*autonomousPrograms)[program];
  while (true) {
    autonomous->update(robot);
  }
}
} // namespace robot::autonomous
