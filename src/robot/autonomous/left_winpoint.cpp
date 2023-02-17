#include "robot/autonomous/left_winpoint.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"

namespace robot::autonomous {
void LeftWinpoint::run(AutonomousContext &context) {
  auto &robot = context.robot;
  spin_roller(context);

//  robot.drivetrain->forwards(5.4);
//  robot.drivetrain->turn_left(10.0);
//  shoot(context, 2, 9500, 500.0);

  robot.drivetrain->forwards(4.5, 80);
  robot.drivetrain->turn_right(90.0);
  robot.flywheel->engage_velocity(470.0);
  robot.drivetrain->forwards(44.0, 200);
  robot.drivetrain->turn_left(90.0);
  robot.drivetrain->forwards(44.0, 200);
  robot.drivetrain->turn_left(45.0);
  shoot(context, 2, 9550, 495.0, false);
}
} // namespace robot::autonomous
