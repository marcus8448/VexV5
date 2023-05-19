#include "control/autonomous/autonomous.hpp"
#include "debug/logger.hpp"

namespace control::autonomous {
static std::map<const std::string, void (*)(robot::Robot &)> *programs =
    new std::map<const std::string, void (*)(robot::Robot &)>();
static const std::string *activeProgram = nullptr;

void initialize() {
  auto *none = new std::string("None");
  registerRun(*none, [](Robot &robot) -> void {});
  activeProgram = none;
}

void registerRun(const std::string &name, void (*function)(robot::Robot &)) {
  info("Registering autonomous '%s'.", name.c_str());
  (*programs)[name] = function;
}

std::map<const std::string, void (*)(robot::Robot &)> &getPrograms() { return *programs; }

const std::string *getActive() { return activeProgram; }

void set_active(const std::string *program) { activeProgram = program; }

void run(Robot &robot) { (*programs)[*activeProgram](robot); }
} // namespace control::autonomous
