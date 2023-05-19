#include "control/autonomous/skills.hpp"
#include "debug/logger.hpp"

namespace control::autonomous {
void skills(Robot &robot) {
  startTiming(skills);
  endTiming(skills);
}
} // namespace control::autonomous
