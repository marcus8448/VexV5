#include "robot/autonomous/left_winpoint.hpp"

namespace robot::autonomous {
void LeftWinpoint::run(Robot *robot) {
  robot->drivetrain->backwards(1.5);
  robot->intake->spin_until_colour(config::get_alliance_colour(), 5000);
  robot->drivetrain->forwards(3.0);
  robot->drivetrain->turn_left(90.0);
  robot->drivetrain->backwards(27.2);
  robot->drivetrain->turn_left(90.0);
  robot->drivetrain->backwards(14);
  robot->intake->engage();
  robot->drivetrain->backwards(12.8, 50.0);
  robot->drivetrain->turn_right(180.0 - 25.7, DRIVETRAIN_MAX_RPM, false);
  robot->flywheel->engage();
  this->wait(3000);
  robot->indexer->cycle();
  robot->flywheel->waitForSpeed();
  robot->indexer->cycle();
  robot->flywheel->waitForSpeed();
  robot->indexer->cycle();
  this->wait(500);
}
} // namespace robot::autonomous
