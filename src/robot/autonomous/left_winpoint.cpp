#include "robot/autonomous/left_winpoint.hpp"

namespace robot::autonomous {
void LeftWinpoint::run(Robot &robot) {
  // robot.drivetrain->backwards(2.5);
  robot.drivetrain->backwards(500.0, 5, 0);
  robot.intake->roll_to_team_colour(config::get_instance()->get_alliance_colour(), 10000);
  robot.drivetrain->stop();
  // robot.drivetrain->forwards(10.0);
  // robot.drivetrain->turn_left(10.0);
  // robot.flywheel->engage();
  // robot.flywheel->wait_for_speed();
  // robot.indexer->cycle();
  // robot.flywheel->wait_for_speed();
  // robot.indexer->cycle();
  // pros::delay(250);
  // robot.flywheel->disengage();
}
} // namespace robot::autonomous
