#include "robot/autonomous/right_winpoint.hpp"
#include "configuration.hpp"

namespace robot::autonomous {
void RightWinpoint::run(Robot &robot) {
  robot.drivetrain->backwards(1.5);
  robot.drivetrain->turn_left(90.0);
  robot.drivetrain->forwards(27.0);
  robot.drivetrain->turn_right(90.0);
  robot.drivetrain->forwards(2.0);
  robot.drivetrain->forwards(5.0, DRIVETRAIN_DEFAULT_RPM / 2.0, 0);
  robot.intake->roll_to_team_colour(config::get_instance()->get_alliance_colour(), 5000);
  robot.flywheel->engage();
  robot.drivetrain->backwards(2.0);
  robot.drivetrain->turn_left(100.0);
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  pros::delay(500);
  robot.flywheel->disengage();
}
} // namespace robot::autonomous
