#include "control/autonomous/replay.hpp"
#include "debug/logger.hpp"
#include "filesystem.hpp"

namespace control::autonomous {
void replay(Robot &robot, const char *name) {
  char *input = static_cast<char *>(fs::read_all(name));

  if (input == nullptr) {
    warn("Failed to open recording with name %s!", name);
    return;
  }
}
} // namespace control::autonomous
