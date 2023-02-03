#include "robot/autonomous/left_winpoint.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"

namespace robot::autonomous {
void LeftWinpoint::run(Robot &robot) {
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
  robot.drivetrain->tare();
  pros::delay(AUTONOMOUS_ROLLER_SPIN_TIME);
  robot.intake->disengage();

  robot.drivetrain->forwards(5.4);
  robot.drivetrain->turn_left(10.0);
  robot.flywheel->engage(10900);
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  pros::delay(250);
  robot.flywheel->disengage();
}
} // namespace robot::autonomous
