#include "robot/autonomous/right_winpoint.hpp"
#include "util.hpp"

namespace robot::autonomous {
void RightWinpoint::run(AutonomousContext &context) {
  auto &robot = context.robot;
  robot.drivetrain->forwards(7.2, 60);

//  robot.drivetrain->turn_right(18.5);
//  robot.drivetrain->forwards(10.0);
//  shoot(context, 2, 9350, 490.0);
//  robot.drivetrain->backwards(10.0);
  robot.drivetrain->turn_right(90.0 /*- 18.5*/);

  robot.drivetrain->forwards(22.0, 70);
  robot.drivetrain->turn_left(90.0, 70);
  robot.drivetrain->backwards(4.0, 50);

  spin_roller(context);
}
} // namespace robot::autonomous
