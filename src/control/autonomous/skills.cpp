#include "skills.hpp"
#include "debug/logger.hpp"

namespace control::autonomous {
void skills(Robot &robot) {
  startTiming(skills);
  robot.drivetrain.backwards(7.0);
  robot.drivetrain.pivotLeft(-45.0, device::Motor::MAX_MILLIVOLTS, false);
  while (std::abs(robot.drivetrain.targetHeading - robot.drivetrain.heading) > 5.0) {
    pros::c::delay(10);
  }
  robot.drivetrain.backwards(100000, 3000, false);
  robot.catapult.launchRepeat();
  pros::c::delay(10000);
  robot.drivetrain.brake();
  robot.drivetrain.tare();

  endTiming(skills);
}
} // namespace control::autonomous
