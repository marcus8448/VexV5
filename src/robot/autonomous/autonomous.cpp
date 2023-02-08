#include "robot/autonomous/autonomous.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"
#include <map>
#include <string>

std::map<const std::string, robot::autonomous::Autonomous *> *autonomousPrograms =
    new std::map<const std::string, robot::autonomous::Autonomous *>();
namespace robot::autonomous {
static const std::string *activeProgram = nullptr;

void Autonomous::rollerBackwards(Robot &robot) {
  double vel = robot.intake->bring_roller_to_speed(6000);
  robot.drivetrain->backwards(5000.0, 20, 0);
  while (robot.intake->get_motor().get_velocity() + AUTONOMOUS_ROLLER_SPIN_THRESHOLD > vel) {
    debug("intake velocity: %f [%f off]", robot.intake->get_motor().get_velocity(),
          robot.intake->get_motor().get_velocity() - vel);
    pros::delay(4);
  }
  debug("intake velocity: %f (%f over)", robot.intake->get_motor().get_velocity(),
        robot.intake->get_motor().get_velocity() - vel);

  robot.drivetrain->brake();
  robot.drivetrain->tare();
  pros::delay(AUTONOMOUS_ROLLER_SPIN_TIME);
  robot.intake->disengage();
}

void Autonomous::shoot(Robot &robot, uint8_t discs, uint16_t millivolts, double velocity) {
  robot.flywheel->engage(millivolts);
  for (int i = 0; i < discs; i++) {
    robot.flywheel->wait_for_speed();
    robot.indexer->cycle();
  }
  pros::delay(250);
  robot.flywheel->disengage();
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
    error("No autonomous selected!");
    return nullptr;
  }
  info("Running autonomous: '%s'", activeProgram->c_str());
  return (*autonomousPrograms)[*activeProgram];
}
} // namespace robot::autonomous
