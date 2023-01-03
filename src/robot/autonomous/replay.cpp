#include "robot/autonomous/replay.hpp"
#include "fs/filesystem.hpp"
#include "pros/rtos.hpp"
#include "logger.hpp"

namespace robot::autonomous {
Replay::Replay(const char *name): Autonomous(name) {}

void Replay::run(Robot &robot) {
  if (!fs::file_exists(name)) {
    error("No recording found with name '%s'!", this->name);
    return;
  }

  std::ifstream input = fs::open(this->name);
}
} // namespace robot::autonomous
