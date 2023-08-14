#include "control/autonomous/autonomous.hpp"
#include "debug/logger.hpp"

namespace control::autonomous {
static std::map<const std::string, void (*)(robot::Robot &)> programs =
    std::map<const std::string, void (*)(robot::Robot &)>();

void initialize() {
  registerRun(std::string("None"), [](Robot &robot) -> void {});
}

void registerRun(const std::string &name, void (*function)(robot::Robot &)) {
  info("Registering autonomous '%s'.", name.c_str());
  programs[name] = function;
}

std::map<const std::string, void (*)(robot::Robot &)> &getPrograms() { return programs; }
} // namespace control::autonomous
