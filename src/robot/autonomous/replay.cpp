#include "robot/autonomous/replay.hpp"
#include "fs/filesystem.hpp"
#include "debug/logger.hpp"

namespace robot::autonomous {
Replay::Replay(const char *name) : Autonomous(name) {}

void Replay::run(Robot &robot) {
  char* input = static_cast<char *>(fs::read_all(this->name));

  if (input == nullptr) {
    warn("Failed to open recording with name %s!", this->name);
    return;
  }
}
} // namespace robot::autonomous
