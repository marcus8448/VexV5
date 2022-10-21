#include "robot/autonomous/right_winpoint.hpp"

namespace robot::autonomous {
void RightWinpoint::run(Robot *robot) {
  robot->drivetrain->backwards(1.5);
  robot->drivetrain->turn_left(90.0);
  robot->drivetrain->forwards(27.0);
  robot->drivetrain->turn_right(90.0);
  robot->drivetrain->forwards(2.0);
  robot->drivetrain->forwards(1, DEFAULT_MAX_RPM / 2.0, false);
  robot->intake->spin_until_colour(config::get_alliance_colour(), 5000);
  robot->flywheel->engage();
  robot->drivetrain->backwards(2.0);
  robot->drivetrain->turn_left(100.0);
  this->wait(2000);
  robot->indexer->cycle();
  robot->flywheel->waitForSpeed();
  robot->indexer->cycle();
}
} // namespace robot::autonomous
