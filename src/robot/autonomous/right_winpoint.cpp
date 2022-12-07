#include "robot/autonomous/right_winpoint.hpp"
#include "configuration.hpp"

namespace robot::autonomous {
void RightWinpoint::run(Robot &robot) {
  robot.drivetrain->backwards(7.2);
  robot.flywheel->engage();
  robot.drivetrain->turn_left(17.0);
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  pros::delay(500);
  robot.flywheel->disengage();
  robot.drivetrain->turn_left(73.0);
  robot.drivetrain->forwards(20.64);
  robot.drivetrain->turn_right(60.0);
  robot.drivetrain->forwards(4.08);
  robot.drivetrain->turn_right(35.0);
  robot.drivetrain->forwards(500.0, DRIVETRAIN_DEFAULT_RPM / 3.0, 0);
  robot.intake->roll_to_team_colour(config::get_instance()->get_alliance_colour(), 5000);
  robot.drivetrain->stop();
}
} // namespace robot::autonomous
