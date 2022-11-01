#include "robot/autonomous/left_winpoint.hpp"

namespace robot::autonomous {
void LeftWinpoint::run(Robot *robot) {
  robot->drivetrain->backwards(2.5);
  robot->drivetrain->backwards(5.0, DRIVETRAIN_DEFAULT_RPM / 2, false);
  robot->intake->hopefully_flip_state(config::get_instance()->get_alliance_colour(), 5000);
  robot->drivetrain->forwards(3.0);
  robot->drivetrain->turn_left(90.0);
  robot->drivetrain->backwards(27.2);
  robot->drivetrain->turn_left(90.0);
  robot->drivetrain->backwards(14);
  robot->intake->engage();
  robot->flywheel->engage();
  robot->drivetrain->backwards(12.8, 50.0);
  robot->drivetrain->turn_right(180.0 - 25.7, DRIVETRAIN_DEFAULT_RPM, false);
  robot->flywheel->waitForSpeed();
  robot->indexer->cycle();
  robot->flywheel->waitForSpeed();
  robot->indexer->cycle();
  robot->flywheel->waitForSpeed();
  robot->indexer->cycle();
  pros::delay(500);
  robot->flywheel->disengage();
}
} // namespace robot::autonomous
