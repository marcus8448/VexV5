#include "robot/autonomous/none.hpp"
#include "logger.hpp"

namespace robot::autonomous {
void None::run(AutonomousContext &context) { warn("No autonomous running!"); }
} // namespace robot::autonomous
