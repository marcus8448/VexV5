#include "robot/autonomous/autonomous.hpp"
#include <map>

std::map<const char *, robot::autonomous::Autonomous *> *autonomousPrograms =
    new std::map<const char *, robot::autonomous::Autonomous *>();
namespace robot::autonomous {
static const char *program = 0;

void register_autonomous(const char *name, Autonomous *program) { (*autonomousPrograms)[name] = program; }

void set_active(const char *program) { robot::autonomous::program = program; }

Autonomous *get_autonomous() {
  return (*autonomousPrograms)[program];
}

} // namespace robot::autonomous
