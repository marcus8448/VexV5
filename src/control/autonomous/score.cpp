#include "score.hpp"

#include <pros/rtos.h>

namespace control::autonomous {
void leftScore(Robot &robot) {
  robot.drivetrain.forwards(40.0);
  robot.drivetrain.turnRight(45.0);
  robot.drivetrain.turnRight(15.0);
}

void rightScore(Robot &robot) {
  robot.drivetrain.forwards(60.0, device::Motor::MAX_MILLIVOLTS, false);
  const uint32_t time = pros::c::millis() + 2000;
  while (!robot.drivetrain.isAtTarget() && pros::c::millis() < time) {
    pros::c::delay(50);
  } // 2000ms total
  robot.drivetrain.backwards(20.0);
}
} // namespace control::autonomous
