#include "control/autonomous/skills.hpp"
#include "debug/logger.hpp"

namespace robot::autonomous {
void Skills::run(Robot &robot) {
  uint32_t t = pros::millis();
  info("Skills done in %ims", pros::millis() - t);
}
} // namespace robot::autonomous
