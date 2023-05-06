#include "control/autonomous/replay.hpp"
#include "debug/logger.hpp"
#include "fs/filesystem.hpp"

namespace control::autonomous {
Replay::Replay(const char *name) : Autonomous(name) {}

void Replay::run(Robot &robot) {
  char *input = static_cast<char *>(fs::read_all(this->name));

  if (input == nullptr) {
    warn("Failed to open recording with name %s!", this->name);
    return;
  }
}
} // namespace control::autonomous
