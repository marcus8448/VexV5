#include "robot/autonomous/none.hpp"
#include "logger.hpp"

namespace robot::autonomous {
void None::run(Robot &robot) { warn("No autonomous running!"); }
} // namespace robot::autonomous
