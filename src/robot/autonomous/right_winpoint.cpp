#include "robot/autonomous/right_winpoint.hpp"
#include "configuration.hpp"
#include "util.hpp"

namespace robot::autonomous {
void RightWinpoint::run(Robot &robot) {
  robot.flywheel->engage(11000);
  robot.drivetrain->forwards(7.2);
  robot.drivetrain->turn_right(90.0);
  robot.drivetrain->turn_right(20.0);
  robot.drivetrain->forwards(10.0);
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  robot.drivetrain->backwards(10.0);
  robot.flywheel->disengage();
  robot.drivetrain->turn_right(73.0);
  robot.drivetrain->forwards(26.0);
  robot.drivetrain->turn_left(90.0);
  robot.drivetrain->backwards(4.0, 50);

  double vel = robot.intake->bring_roller_to_speed(5500);
  robot.drivetrain->backwards(500.0, 5, 0);
  while (robot.intake->get_motor().get_velocity() > vel) {
    pros::delay(10);
  }
  robot.drivetrain->stop();
  pros::delay(AUTONOMOUS_ROLLER_SPIN_TIME);
  robot.intake->disengage();
}
} // namespace robot::autonomous
