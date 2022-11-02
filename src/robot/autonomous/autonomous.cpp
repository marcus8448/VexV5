#include "robot/autonomous/autonomous.hpp"
#include "logger.hpp"
#include <map>
#include <string>

std::map<const std::string, robot::autonomous::Autonomous *> *autonomousPrograms =
    new std::map<const std::string, robot::autonomous::Autonomous *>();
namespace robot::autonomous {
static const char *activeProgram = nullptr;

void register_autonomous(const char *name, Autonomous *program) { register_autonomous(std::string(name), program); }

void register_autonomous(const std::string name, Autonomous *program) {
  logger::info("Registering autonomous '%s'.", name);
  (*autonomousPrograms)[name] = program;

  if (activeProgram == nullptr) {
    logger::info("Setting '%s' as the default autonomous.", name);
    activeProgram = name.c_str(); // set the default program to the first one registered
  }
}

void set_active(const char *program) { activeProgram = program; }

void set_active(const std::string program) { set_active(program.c_str()); }

Autonomous *get_autonomous() {
  if (activeProgram == nullptr) {
    logger::error("No autonomous selected!");
    return nullptr;
  }
  return (*autonomousPrograms)[std::string(activeProgram)];
}
} // namespace robot::autonomous
