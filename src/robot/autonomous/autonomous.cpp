#include "robot/autonomous/autonomous.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"
#include <map>
#include <string>
#include <deque>

#define AUTONOMOUS_ROLLER_SPIN_THRESHOLD 40

std::map<const std::string, robot::autonomous::Autonomous *> *autonomousPrograms =
    new std::map<const std::string, robot::autonomous::Autonomous *>();
namespace robot::autonomous {
static const std::string *activeProgram = nullptr;

AutonomousContext::AutonomousContext(Robot &robot) : robot(robot) {}

void spin_roller(AutonomousContext &context, uint32_t length, int16_t mV) {
  uint32_t time = pros::millis();
  Robot &robot = context.robot;
  double vel = robot.intake->bring_roller_to_speed(11000);
  robot.drivetrain->power(-20.0);
  while (robot.intake->get_motor().get_velocity() + AUTONOMOUS_ROLLER_SPIN_THRESHOLD > vel || robot.intake->get_motor().get_velocity() < 20.0) {
    debug("intake velocity: %f [%f off]", robot.intake->get_motor().get_velocity(),
          robot.intake->get_motor().get_velocity() - vel);
    pros::delay(4);
  }
  info("PASS: intake velocity: %f [%f over]", robot.intake->get_motor().get_velocity(),
        robot.intake->get_motor().get_velocity() - vel);
  info("Took %ims to get roller up to speed.", pros::millis() - time);

  pros::delay(length);
  robot.drivetrain->brake();
  robot.drivetrain->tare();
  robot.intake->disengage();
}

void shoot(AutonomousContext &context, uint8_t discs, int16_t millivolts, double velocity, bool preSpeed) {
  debug("Launching %i discs ar %imV (%f vel)", discs, millivolts, velocity);
  uint32_t time = pros::millis();
  Robot &robot = context.robot;
  robot.flywheel->reset_speeds();
//  if (robot.flywheel->get_primary_motor().get_velocity() < 100.0 && millivolts > 9000 && preSpeed) {
//    robot.flywheel->engage(12000);
//    double prev[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
//    while (true) {
//      double d = (prev[0] + prev[1] + prev[2] + prev[3] + prev[4]) / 5.0;
//      prev[4] = prev[3];
//      prev[3] = prev[2];
//      prev[2] = prev[1];
//      prev[1] = prev[0];
//
//      prev[0] = robot.flywheel->get_velocity();
//
//      if (prev[4] == 0.0)
//        continue;
//
//      d = prev[0] - d;
//      debug("d12 %f / %f", prev[0] + d * 2.0, velocity);
//      if (prev[0] + d * 2.5 >= velocity - 12.0 /* || std::abs(prev[0] - velocity) < 20.0*/) {
//        robot.flywheel->engage(millivolts);
//        break;
//      }
//      pros::delay(3);
//    }
//  } else {
//    robot.flywheel->engage(millivolts);
//  }
  robot.flywheel->engage_velocity(velocity);
  for (uint8_t i = 0; i < discs; i++) {
    uint32_t t = pros::millis();
    robot.flywheel->wait_for_speed(velocity);
    info("Launched at %f (took %ims)", robot.flywheel->get_velocity(), t - pros::millis());
    robot.indexer->cycle();
  }
  pros::delay(100);
  robot.flywheel->disengage();
  info("Launching %i discs took %ims", discs, pros::millis() - time);
}

Autonomous::Autonomous(const char *name) : name(name) {}

const char *Autonomous::get_name() { return this->name; }

void register_autonomous(Autonomous *program) {
  info("Registering autonomous '%s'.", program->get_name());
  (*autonomousPrograms)[std::string(program->get_name())] = program;

  if (activeProgram == nullptr) {
    info("Setting '%s' as the default autonomous.", program->get_name());
    activeProgram = new std::string(program->get_name()); // set the default program to the first one registered
  }
}

const std::string *get_active() { return activeProgram; }

void set_active(const std::string *program) { activeProgram = program; }

Autonomous *get_autonomous() {
  if (activeProgram == nullptr) {
    return nullptr;
  }
  return (*autonomousPrograms)[*activeProgram];
}

void run_autonomous(Autonomous *autonomous, Robot &robot) {
  if (activeProgram == nullptr) {
    error("No autonomous to run!");
    return;
  }
  info("Running autonomous: '%s'", activeProgram->c_str());
  auto context = AutonomousContext(robot);
  autonomous->run(context);
}
} // namespace robot::autonomous
