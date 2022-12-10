#include "robot/autonomous/autonomous.hpp"
#include "logger.hpp"
#include <map>
#include <string>

std::map<const std::string, robot::autonomous::Autonomous *> *autonomousPrograms =
    new std::map<const std::string, robot::autonomous::Autonomous *>();
namespace robot::autonomous {
static const std::string *activeProgram = nullptr;

void register_autonomous(const char *name, Autonomous *program) {
  logger::info("Registering autonomous '%s'.", name);
  (*autonomousPrograms)[std::string(name)] = program;

  if (activeProgram == nullptr) {
    logger::info("Setting '%s' as the default autonomous.", name);
    activeProgram = new std::string(name); // set the default program to the first one registered
  }
}

const std::string *get_active() { return activeProgram; }

void set_active(const std::string *program) { activeProgram = program; }

Autonomous *get_autonomous() {
  if (activeProgram == nullptr) {
    logger::error("No autonomous selected!");
    return nullptr;
  }
  logger::info("Running autonomous: '%s'", activeProgram);
  return (*autonomousPrograms)[*activeProgram];
}
} // namespace robot::autonomous
