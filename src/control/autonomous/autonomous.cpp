#include "control/autonomous/autonomous.hpp"
#include "debug/logger.hpp"

namespace control::autonomous {
static std::unordered_map<std::string, std::function<void(robot::Robot &)>> programs =
    std::unordered_map<std::string, std::function<void(robot::Robot &)>>();

void initialize() {
  registerRun(std::string("None"), [](Robot &robot) {});
}

void registerRun(const std::string &name, std::function<void(robot::Robot &)> function) {
  logger::info("Registering autonomous '%s'.", name.c_str());
  programs[name] = std::move(function);
}

std::unordered_map<std::string, std::function<void(robot::Robot &)>> &getPrograms() { return programs; }
} // namespace control::autonomous
