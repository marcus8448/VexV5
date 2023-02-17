#include "robot/autonomous/left_skills.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"

namespace robot::autonomous {
void LeftSkills::run(AutonomousContext &context) {
  uint32_t t = pros::millis();
  auto &robot = context.robot;
  spin_roller(context, AUTONOMOUS_ROLLER_OPPOSITE_SPIN_TIME);
  robot.drivetrain->forwards(22.0, 150);
  robot.drivetrain->backwards(2.0);
  robot.intake->engage();
  robot.drivetrain->turn_right(90.0);
  robot.drivetrain->backwards(19.0, 100);
  pros::delay(1000);
  robot.intake->disengage();
  pros::delay(500);
  spin_roller(context, AUTONOMOUS_ROLLER_OPPOSITE_SPIN_TIME);
  robot.drivetrain->forwards(6.0);
  robot.drivetrain->turn_left(90.0);
  robot.drivetrain->forwards(48.0, 150);
  shoot(context, 3, 8200, 420.0);

//  robot.drivetrain->backwards(3.6);
  robot.drivetrain->turn_left(45);
  robot.intake->engage();
  robot.drivetrain->backwards(45.0, 125);
  robot.drivetrain->forwards(41.0, 150);
  robot.drivetrain->turn_right(41);
//  robot.drivetrain->forwards(3.6);
  robot.intake->disengage();
  shoot(context, 3, 8200, 420.0);

  robot.drivetrain->backwards(7.2);
  robot.drivetrain->turn_left(90);
  robot.intake->engage();
  robot.drivetrain->backwards(26.88, 150);
  robot.drivetrain->turn_right(45);
  robot.drivetrain->backwards(15.0);
  robot.drivetrain->turn_left(135);
  robot.drivetrain->backwards(24.0, 100);
  robot.drivetrain->turn_right(135);
  robot.intake->disengage();
  shoot(context, 3, 8200, 420.0);
  robot.drivetrain->turn_left(90.0);
  info("MS: %i", pros::millis() - t);
  robot.drivetrain->forwards(50.0, 200);
  robot.expansion->launch();
//  robot.drivetrain->backwards(19.0, 100);

//  robot.drivetrain->turn_left(45);
//  robot.intake->engage();
//  robot.drivetrain->backwards(24.48);
//  robot.drivetrain->turn_right(90);
//  robot.drivetrain->backwards(23.04);
//  robot.drivetrain->turn_left(135);
//  robot.drivetrain->backwards(12.0);
//  robot.drivetrain->turn_left(75);
//  robot.drivetrain->forwards(5.76);
//  robot.intake->disengage();
//  shoot(context, 3, 8200, 420.0);
//  pros::delay(250);
//  robot.flywheel->disengage();
//
//  robot.drivetrain->backwards(5.76);
//  robot.drivetrain->turn_right(75);
//  robot.intake->engage();
//  robot.drivetrain->backwards(24.0);
//  robot.drivetrain->turn_left(45);
//  robot.drivetrain->backwards(33.6);
//  robot.intake->disengage();
//  spin_roller(context, AUTONOMOUS_ROLLER_OPPOSITE_SPIN_TIME);
//  robot.drivetrain->forwards(17.28);
//  robot.drivetrain->turn_right(90);
//  robot.drivetrain->backwards(11.52);
//  spin_roller(context, AUTONOMOUS_ROLLER_OPPOSITE_SPIN_TIME);
//  robot.drivetrain->forwards(11.04);
//  robot.drivetrain->turn_right(90.0);
  info("Skills done in %ims", pros::millis() - t);
}
} // namespace robot::autonomous
