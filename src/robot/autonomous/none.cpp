#include "robot/autonomous/none.hpp"
#include "logger.hpp"

namespace robot::autonomous {
void None::run(Robot &robot) { logger::warn("No autonomous"); }
} // namespace robot::autonomous
