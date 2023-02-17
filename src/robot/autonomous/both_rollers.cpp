#include "robot/autonomous/both_rollers.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"

namespace robot::autonomous {
void BothRollers::run(AutonomousContext &context) {
  auto &robot = context.robot;
  spin_roller(context);

  robot.drivetrain->forwards(4.5, 80);
  robot.drivetrain->turn_right(90.0);
  robot.drivetrain->forwards(16.0, 200);
  robot.drivetrain->turn_left(45.0);
  robot.drivetrain->forwards(115.0, 150);
  robot.drivetrain->turn_left(45.0);
  robot.drivetrain->forwards(16.0, 200);
  robot.drivetrain->turn_left(90.0);
  spin_roller(context);
}
} // namespace robot::autonomous
