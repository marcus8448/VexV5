#include "robot/autonomous/right_winpoint.hpp"
#include "configuration.hpp"
#include "logger.hpp"
#include "util.hpp"

namespace robot::autonomous {
void RightWinpoint::run(Robot &robot) {
  robot.flywheel->engage(11000);
  robot.drivetrain->forwards(7.2);
  robot.drivetrain->turn_right(90.0);
  // robot.drivetrain->turn_right(20.0);
  // robot.drivetrain->forwards(10.0);
  // robot.flywheel->wait_for_speed();
  // robot.indexer->cycle();
  // robot.flywheel->wait_for_speed();
  // robot.indexer->cycle();
  // robot.drivetrain->backwards(10.0);
  // robot.flywheel->disengage();
  // robot.drivetrain->turn_right(73.0);
  robot.drivetrain->forwards(26.0);
  robot.drivetrain->turn_left(90.0);
  robot.drivetrain->backwards(4.0, 50);
  robot.drivetrain->backwards(500.0, 5, 0);
  robot.intake->roll_to_team_colour(config::get_instance()->get_alliance_colour(), 10000);
  robot.drivetrain->stop();
}
} // namespace robot::autonomous
