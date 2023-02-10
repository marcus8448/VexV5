#include "robot/autonomous/left_skills.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"

namespace robot::autonomous {
void LeftSkills::run(AutonomousContext &context) {
  auto &robot = context.robot;
  robot.drivetrain->forwards(14.4);
  robot.drivetrain->turn_right(90.0);
  robot.drivetrain->backwards(21.168);
  rollerBackwards(context);
  robot.drivetrain->forwards(6.912);
  robot.drivetrain->turn_left(90);
  robot.drivetrain->forwards(53.088);
  shoot(context, 2, 10900, 450.0);

  robot.drivetrain->backwards(3.6);
  robot.drivetrain->turn_left(45);
  robot.intake->engage();
  robot.drivetrain->backwards(24.576);
  robot.intake->disengage();
  robot.drivetrain->forwards(24.576);
  robot.drivetrain->turn_right(45);
  robot.drivetrain->forwards(3.6);
  shoot(context, 2, 10900, 450.0);
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  pros::delay(250);
  robot.flywheel->disengage();

  robot.drivetrain->backwards(7.2);
  robot.drivetrain->turn_left(90);
  robot.intake->engage();
  robot.drivetrain->backwards(26.88);
  robot.drivetrain->turn_right(45);
  robot.drivetrain->backwards(12.0);
  robot.drivetrain->turn_left(135);
  robot.drivetrain->backwards(24.0);
  robot.intake->disengage();
  robot.drivetrain->turn_right(135);
  shoot(context, 3, 11500, 500.0);

  robot.drivetrain->turn_left(45);
  robot.intake->engage();
  robot.drivetrain->backwards(24.48);
  robot.drivetrain->turn_right(90);
  robot.drivetrain->backwards(23.04);
  robot.drivetrain->turn_left(135);
  robot.drivetrain->backwards(12.0);
  robot.intake->disengage();
  robot.drivetrain->turn_left(75);
  robot.drivetrain->forwards(5.76);
  shoot(context, 3, 10900, 450.0);
  robot.flywheel->engage(10900);
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  robot.flywheel->wait_for_speed();
  robot.indexer->cycle();
  pros::delay(250);
  robot.flywheel->disengage();

  robot.drivetrain->backwards(5.76);
  robot.drivetrain->turn_right(75);
  robot.intake->engage();
  robot.drivetrain->backwards(24.0);
  robot.drivetrain->turn_left(45);
  robot.drivetrain->backwards(33.6);
  robot.intake->disengage();
  rollerBackwards(context);
  robot.drivetrain->forwards(17.28);
  robot.drivetrain->turn_right(90);
  robot.drivetrain->backwards(11.52);
  rollerBackwards(context);
  robot.drivetrain->forwards(11.04);
}
} // namespace robot::autonomous
