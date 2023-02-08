#include "robot/autonomous/left_winpoint.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"

namespace robot::autonomous {
void LeftWinpoint::run(Robot &robot) {
  rollerBackwards(robot);

  robot.drivetrain->forwards(5.4);
  robot.drivetrain->turn_left(10.0);
  shoot(robot, 2, 10900, 450.0);
}
} // namespace robot::autonomous
