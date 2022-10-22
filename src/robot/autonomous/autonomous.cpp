#include "robot/autonomous/autonomous.hpp"
#include <map>

std::map<const char *, robot::autonomous::Autonomous *> *autonomousPrograms =
    new std::map<const char *, robot::autonomous::Autonomous *>();
namespace robot::autonomous {
static const char *activeProgram = nullptr;

void register_autonomous(const char *name, Autonomous *program) {
  if (activeProgram == nullptr)
    activeProgram = name; // set the default program to the first one registered
  (*autonomousPrograms)[name] = program;
}

void set_active(const char *program) { activeProgram = program; }

Autonomous *get_autonomous() {
  if (activeProgram == nullptr)
    return nullptr;
  return (*autonomousPrograms)[activeProgram];
}
} // namespace robot::autonomous
