#include "robot/autonomous/right_winpoint.hpp"
#include "logger.hpp"
#include "util.hpp"

namespace robot::autonomous {
void RightWinpoint::run(Robot &robot) {
  robot.flywheel->engage(10000);
  robot.drivetrain->forwards(7.2);
  robot.drivetrain->turn_right(14.0);
  robot.drivetrain->forwards(10.0);
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  robot.drivetrain->backwards(10.0);
  robot.flywheel->disengage();
  robot.drivetrain->turn_right(76.0);
  robot.drivetrain->forwards(22.0);
  robot.drivetrain->turn_left(90.0);
  robot.drivetrain->backwards(4.0, 50);

  double vel = robot.intake->bring_roller_to_speed(6000);
  robot.drivetrain->backwards(5000.0, 20, 0);
  while (robot.intake->get_motor().get_velocity() + AUTONOMOUS_ROLLER_SPIN_THRESHOLD > vel) {
    debug("intake velocity: %f [%f off]", robot.intake->get_motor().get_velocity(),
          robot.intake->get_motor().get_velocity() - vel);
    pros::delay(4);
  }
  debug("intake velocity: %f (%f over)", robot.intake->get_motor().get_velocity(),
        robot.intake->get_motor().get_velocity() - vel);

  robot.drivetrain->brake();
  pros::delay(AUTONOMOUS_ROLLER_SPIN_TIME);
  robot.intake->disengage();
}
} // namespace robot::autonomous
