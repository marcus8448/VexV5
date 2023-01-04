#include "robot/autonomous/autonomous.hpp"
#include "logger.hpp"
#include <map>
#include <string>

std::map<const std::string, robot::autonomous::Autonomous *> *autonomousPrograms =
    new std::map<const std::string, robot::autonomous::Autonomous *>();
namespace robot::autonomous {
static const std::string *activeProgram = nullptr;

Autonomous::Autonomous(const char *name) : name(name) {}

const char *Autonomous::get_name() { return this->name; }

void register_autonomous(Autonomous *program) {
  info("Registering autonomous '%s'.", program->get_name());
  (*autonomousPrograms)[std::string(program->get_name())] = program;

  if (activeProgram == nullptr) {
    info("Setting '%s' as the default autonomous.", program->get_name());
    activeProgram = new std::string(program->get_name()); // set the default program to the first one registered
  }
}

const std::string *get_active() { return activeProgram; }

void set_active(const std::string *program) { activeProgram = program; }

Autonomous *get_autonomous() {
  if (activeProgram == nullptr) {
    error("No autonomous selected!");
    return nullptr;
  }
  info("Running autonomous: '%s'", activeProgram);
  return (*autonomousPrograms)[*activeProgram];
}
} // namespace robot::autonomous
