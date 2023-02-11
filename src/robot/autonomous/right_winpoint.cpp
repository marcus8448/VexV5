#include "robot/autonomous/right_winpoint.hpp"
#include "util.hpp"

namespace robot::autonomous {
void RightWinpoint::run(AutonomousContext &context) {
  auto &robot = context.robot;
  robot.flywheel->engage(10000);
  robot.drivetrain->forwards(7.2);
  robot.drivetrain->turn_right(14.0);
  robot.drivetrain->forwards(10.0);
  shoot(context, 2, 10000, 450.0);
  robot.drivetrain->backwards(10.0);
  robot.drivetrain->turn_right(76.0);
  robot.drivetrain->forwards(22.0);
  robot.drivetrain->turn_left(90.0);
  robot.drivetrain->backwards(4.0, 50);

  spin_roller(context);
}
} // namespace robot::autonomous
