#include "skills.hpp"
#include "debug/logger.hpp"

namespace control::autonomous {
void skills(Robot &robot) {
  startTiming(skills);
  robot.drivetrain.pivotLeft(-45.0, device::Motor::MAX_MILLIVOLTS, false);
  const uint32_t time = pros::c::millis() + 500;
  while (!robot.drivetrain.isAtTarget() && pros::c::millis() < time) {
    pros::c::delay(50);
  } // 500ms
  robot.drivetrain.backwards(100000, 1600, false);
  pros::c::delay(500); // 1000ms
  robot.catapult.launch(40, 12000, 1250, true); // 38500ms
  robot.drivetrain.brake();
  robot.drivetrain.pivotLeft(40.0);
  robot.drivetrain.forwards(80.0);
  robot.drivetrain.pivotRight(90.0 + 30.0);
  robot.drivetrain.forwards(48.0);
  robot.drivetrain.pivotLeft(100.0);
  robot.wings.open();
  robot.drivetrain.forwards(36.0, device::Motor::MAX_MILLIVOLTS, false);
  pros::c::delay(1000);
  robot.drivetrain.backwards(10.0, device::Motor::MAX_MILLIVOLTS, false);
  pros::c::delay(1000);
  robot.drivetrain.forwards(10.0, device::Motor::MAX_MILLIVOLTS, false);
  pros::c::delay(1000);
  robot.drivetrain.backwards(10.0);

  endTiming(skills);
}
} // namespace control::autonomous
