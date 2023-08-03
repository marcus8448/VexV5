#include "control/autonomous/score.hpp"

namespace control::autonomous {
void leftScore(Robot &robot) {
  robot.drivetrain.forwards(44.0);
  robot.drivetrain.turnLeft(45.0);
  robot.intake.intake();
  robot.drivetrain.forwards(15.0);
  robot.drivetrain.turnRight(45.0 + 90.0);
  robot.drivetrain.forwards(24.0);
  robot.intake.release();
  robot.drivetrain.backwards(12.0);
  robot.drivetrain.forwards(12.0);
  robot.drivetrain.backwards(12.0);
  robot.intake.stop();
  robot.drivetrain.turnRight(180.0);
  robot.drivetrain.forwards(40.0 - 12.0);
  robot.drivetrain.turnRight(45.0);
  robot.intake.intake();
  robot.drivetrain.forwards(8.0);
  robot.drivetrain.backwards(12.0);
  robot.drivetrain.turnRight(45.0 + 90.0);
  robot.drivetrain.forwards(40.0 + 12.0);
  robot.intake.release();
  robot.drivetrain.backwards(12.0);
  robot.drivetrain.forwards(12.0);
  robot.drivetrain.backwards(12.0);
  robot.drivetrain.forwards(12.0);
}
void rightScore(Robot &robot) {}
} // namespace control::autonomous
