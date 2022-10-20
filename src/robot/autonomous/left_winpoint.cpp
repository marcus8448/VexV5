#include "robot/autonomous/left_winpoint.hpp"

namespace robot::autonomous {
void LeftWinpoint::run(Robot *robot) {
  robot->drivetrain->forwards(1.5);
  robot->roller->spin_until_colour(config::get_alliance_colour(), 5000);
  robot->drivetrain->backwards(3.0);
  robot->drivetrain->turn_right(90.0);
  robot->drivetrain->backwards(27.2);
  robot->drivetrain->turn_left(90.0);
  robot->drivetrain->backwards(14);
  robot->intake->engage();
  robot->drivetrain->backwards(12.8, 50.0);
  robot->drivetrain->turn_right(180.0 - 25.7, DEFAULT_MAX_RPM, false);
  robot->flywheel->engage();
  this->wait(3000);
  robot->indexer->cycle();
  robot->flywheel->waitForSpeed();
  robot->indexer->cycle();
  robot->flywheel->waitForSpeed();
  robot->indexer->cycle();
}
} // namespace robot::autonomous
