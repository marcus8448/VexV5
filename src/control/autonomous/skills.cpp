#include "skills.hpp"
#include "debug/logger.hpp"

namespace control::autonomous {
void skills(Robot &robot) {
  startTiming(skills);
  robot.drivetrain.pivotRight(-32.0, device::Motor::MAX_MILLIVOLTS, false);
  const uint32_t time = pros::c::millis() + 500;
  while (!robot.drivetrain.isAtTarget() && pros::c::millis() < time) {
    pros::c::delay(50);
  } // 500ms total
  robot.drivetrain.backwards(100000, 1600, false);
  pros::c::delay(500); // 1000ms total
  robot.catapult.launch(40, 12000, 1000, true); // 41000ms total
  robot.drivetrain.brake();
  robot.drivetrain.heading = robot.drivetrain.targetHeading;
  robot.drivetrain.pivotRight(30.0, device::Motor::MAX_MILLIVOLTS, true);
  robot.drivetrain.forwards(80.0, device::Motor::MAX_MILLIVOLTS, true);
  robot.drivetrain.pivotLeft(90.0 + 30.0, device::Motor::MAX_MILLIVOLTS, true);
  robot.drivetrain.forwards(48.0, device::Motor::MAX_MILLIVOLTS, true);
  robot.drivetrain.pivotRight(110.0, device::Motor::MAX_MILLIVOLTS, true);
  robot.wings.open();
  robot.drivetrain.forwards(36.0, device::Motor::MAX_MILLIVOLTS, false);
  pros::c::delay(1250);
  robot.drivetrain.backwards(36.0, 10000, false);
  pros::c::delay(500);
  robot.drivetrain.forwards(36.0, device::Motor::MAX_MILLIVOLTS, false);
  pros::c::delay(700);
  robot.drivetrain.backwards(20.0);
  robot.wings.close();
  endTiming(skills);
}
} // namespace control::autonomous
