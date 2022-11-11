#include "robot/autonomous/left_winpoint.hpp"

namespace robot::autonomous {
void LeftWinpoint::run(Robot &robot) {
  robot.drivetrain->backwards(2.5);
  robot.drivetrain->await_move();
  robot.drivetrain->backwards(5.0, DRIVETRAIN_DEFAULT_RPM / 2);
  robot.intake->roll_to_team_colour(config::get_instance()->get_alliance_colour(), 5000);
  robot.drivetrain->forwards(3.0);
  robot.drivetrain->turn_left(90.0);
  robot.drivetrain->await_move();
  robot.drivetrain->backwards(27.2);
  robot.drivetrain->turn_left(90.0);
  robot.drivetrain->await_move();
  robot.drivetrain->backwards(14);
  robot.drivetrain->await_move();
  robot.intake->engage();
  robot.flywheel->engage();
  robot.drivetrain->backwards(12.8, 50.0);
  robot.drivetrain->turn_right(180.0 - 25.7, DRIVETRAIN_DEFAULT_RPM);
  robot.drivetrain->await_move();
  robot.flywheel->waitForSpeed();
  robot.indexer->cycle();
  robot.flywheel->waitForSpeed();
  robot.indexer->cycle();
  robot.flywheel->waitForSpeed();
  robot.indexer->cycle();
  pros::delay(500);
  robot.flywheel->disengage();
}
} // namespace robot::autonomous
