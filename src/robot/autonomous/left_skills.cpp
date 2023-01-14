#include "robot/autonomous/left_skills.hpp"

namespace robot::autonomous {
void LeftSkills::run(Robot &robot) {
  robot.drivetrain->forwards(12.0);
}
} // namespace robot::autonomous
