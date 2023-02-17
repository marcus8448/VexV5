#include "robot/autonomous/manual_load_skills.hpp"
#include "pros/rtos.hpp"

namespace robot::autonomous {
void ManualLoadSkills::run(AutonomousContext &context) {
  auto &robot = context.robot;
//  robot.flywheel->engage(8700);
  robot.drivetrain->forwards(48.0, 100);
  robot.drivetrain->turn_left(12.0);
  robot.indexer->cycle();
  robot.flywheel->wait_for_speed(5000);
  robot.indexer->cycle();
  robot.drivetrain->turn_right(12.0);
  robot.flywheel->disengage();
  robot.drivetrain->backwards(48.0, 100);
  pros::delay(4000);
  robot.drivetrain->forwards(60.0, 100);
  robot.drivetrain->backwards(60.0, 100);
  pros::delay(4000);
  robot.drivetrain->forwards(60.0, 100);
  robot.drivetrain->backwards(60.0, 100);
  pros::delay(4000);
  robot.drivetrain->forwards(60.0, 100);
  robot.drivetrain->backwards(60.0, 100);
  pros::delay(4000);
  robot.drivetrain->forwards(60.0, 100);
  robot.drivetrain->backwards(60.0, 100);
  pros::delay(4000);
  robot.drivetrain->forwards(60.0, 100);
  robot.drivetrain->turn_left(45.0);
  robot.drivetrain->forwards(7.0);
}
} // namespace robot::autonomous
