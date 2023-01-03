#include "robot/autonomous/left_winpoint.hpp"
#include "pros/rtos.hpp"

namespace robot::autonomous {
void LeftWinpoint::run(Robot &robot) {
  double vel = robot.intake->bring_roller_to_speed(5500);
  robot.drivetrain->backwards(500.0, 5, 0);
  while (robot.intake->get_motor().get_velocity() > vel) {
    pros::delay(10);
  }
  robot.drivetrain->stop();
  pros::delay(AUTONOMOUS_ROLLER_SPIN_TIME);
  robot.intake->disengage();

  robot.drivetrain->forwards(10.0);
  robot.drivetrain->turn_left(10.0);
  robot.flywheel->engage(11000);
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  pros::delay(250);
  robot.flywheel->disengage();
}
} // namespace robot::autonomous
