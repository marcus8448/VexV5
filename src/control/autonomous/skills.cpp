#include "skills.hpp"
#include "debug/logger.hpp"

namespace control::autonomous {
void skills(Robot &robot) {
  startTiming(skills);
  robot.drivetrain.backwards(6.5);
  robot.drivetrain.pivotLeft(-45.0, device::Motor::MAX_MILLIVOLTS, false);
  robot.drivetrain.backwards(100000, 3000, false);
  pros::c::delay(500);
  robot.catapult.launch(30, 12000, 500, true);
  robot.drivetrain.brake();
  pros::c::delay(100);
  robot.drivetrain.tare();

  endTiming(skills);
}
} // namespace control::autonomous
