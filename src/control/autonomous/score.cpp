#include "control/autonomous/score.hpp"

namespace control::autonomous {
void leftScore(Robot &robot) {
  robot.drivetrain.forwards(40.0);
  robot.drivetrain.turnRight(45.0);
  robot.drivetrain.turnRight(15.0);
}

void rightScore(Robot &robot) {
  robot.drivetrain.forwards(52.0, 8000);
  robot.drivetrain.pivotRightAbs(90.0, 5000);
  robot.drivetrain.forwards(17.0, 12000);
  robot.drivetrain.backwards(22.0, 10000);
//  robot.drivetrain.forwards(22.0, 12000);
//  robot.drivetrain.backwards(22.0, 10000);
//  robot.drivetrain.pivotRightAbs(-24.0, 12000);
//  robot.drivetrain.forwards(16.0, 2500);
//  robot.drivetrain.pivotRightAbs(100.0, 2000);
//  robot.drivetrain.forwards(18.0, 12000);
}
} // namespace control::autonomous
