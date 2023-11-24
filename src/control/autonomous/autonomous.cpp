#include "control/autonomous/autonomous.hpp"
#include "debug/logger.hpp"

namespace control::autonomous {
static std::unordered_map<std::string, std::function<void(Robot &)>> programs =
    std::unordered_map<std::string, std::function<void(Robot &)>>();

void initialize() {
  registerRun(std::string("None"), [](Robot &_) {});
}

void registerRun(const std::string &name, std::function<void(Robot &)> function) {
  logger::info("Registering autonomous '{}'.", name.c_str());
  programs[name] = std::move(function);
}

std::unordered_map<std::string, std::function<void(Robot &)>> &getPrograms() { return programs; }
} // namespace control::autonomous
