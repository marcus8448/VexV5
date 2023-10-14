#include "control/autonomous/score.hpp"

namespace control::autonomous {
void leftScore(Robot &robot) {
  robot.drivetrain.forwards(53.0);
  robot.drivetrain.turnRight(90.0);
}

void rightScore(Robot &robot) {
  robot.drivetrain.forwards(48.0, 8000);
  robot.drivetrain.pivotRightAbs(90.0, 5000);
  robot.drivetrain.forwards(14.0, 12000);
  robot.drivetrain.backwards(10.0, 11000);
  robot.drivetrain.pivotRightAbs(-24.0, 12000);
  robot.drivetrain.forwards(16.0, 4000);
  robot.drivetrain.pivotRightAbs(90.0, 3000);
  robot.drivetrain.forwards(18.0, 12000);
}
} // namespace control::autonomous
