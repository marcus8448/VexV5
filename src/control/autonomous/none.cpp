#include "control/autonomous/none.hpp"

namespace control::autonomous {
void None::run(Robot &robot) { warn("No autonomous running!"); }
} // namespace control::autonomous
