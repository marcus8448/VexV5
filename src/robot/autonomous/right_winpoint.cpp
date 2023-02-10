#include "robot/autonomous/right_winpoint.hpp"
#include "logger.hpp"
#include "util.hpp"

namespace robot::autonomous {
void RightWinpoint::run(AutonomousContext &context) {
  auto &robot = context.robot;
  robot.flywheel->engage(10000);
  robot.drivetrain->forwards(7.2);
  robot.drivetrain->turn_right(14.0);
  robot.drivetrain->forwards(10.0);
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  robot.drivetrain->backwards(10.0);
  robot.flywheel->disengage();
  robot.drivetrain->turn_right(76.0);
  robot.drivetrain->forwards(22.0);
  robot.drivetrain->turn_left(90.0);
  robot.drivetrain->backwards(4.0, 50);

  rollerBackwards(context);
}
} // namespace robot::autonomous
