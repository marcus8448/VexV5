#include "robot/autonomous/autonomous.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"
#include <map>
#include <string>

std::map<const std::string, robot::autonomous::Autonomous *> *autonomousPrograms =
    new std::map<const std::string, robot::autonomous::Autonomous *>();
namespace robot::autonomous {
static const std::string *activeProgram = nullptr;
pros::task_t *async_task = nullptr;

AutonomousContext::AutonomousContext(Robot &robot): robot(robot) {
}

void AutonomousContext::add_async_task(bool (*function)(Robot &)) {
  this->async_functions.emplace_back(function);
}

void Autonomous::rollerBackwards(AutonomousContext &context) {
  Robot &robot = context.robot;
  double vel = robot.intake->bring_roller_to_speed(6000);
  robot.drivetrain->backwards(5000.0, 20, 0);
  while (robot.intake->get_motor().get_velocity() + AUTONOMOUS_ROLLER_SPIN_THRESHOLD > vel) {
    debug("intake velocity: %f [%f off]", robot.intake->get_motor().get_velocity(),
          robot.intake->get_motor().get_velocity() - vel);
    pros::delay(4);
  }
  debug("intake velocity: %f [%f over]", robot.intake->get_motor().get_velocity(),
        robot.intake->get_motor().get_velocity() - vel);

  robot.drivetrain->brake();
  robot.drivetrain->tare();
  pros::delay(AUTONOMOUS_ROLLER_SPIN_TIME);
  robot.intake->disengage();
}

void Autonomous::shoot(AutonomousContext &context, uint8_t discs, int16_t millivolts, double velocity) {
  Robot &robot = context.robot;
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

void async_auto_task(void *param) {
  AutonomousContext &context = *static_cast<AutonomousContext *>(param);
  while (true) {
    size_t size = context.async_functions.size();
    for (int i = 0; i < size; i++) {
      if (context.async_functions[i](context.robot)) {
        context.async_functions.erase(context.async_functions.begin() + i--);
        size--;
      }
    }
  }
}

Autonomous *get_autonomous() {
  if (activeProgram == nullptr) {
    return nullptr;
  }
  return (*autonomousPrograms)[*activeProgram];
}

void run_autonomous(Autonomous* autonomous, Robot &robot) {
  if (activeProgram == nullptr) {
    error("No autonomous to run!");
    return;
  }
  info("Running autonomous: '%s'", activeProgram->c_str());
  auto context = AutonomousContext(robot);
  async_task = static_cast<pros::task_t *>(malloc(sizeof(pros::task_t)));
  *async_task = pros::c::task_create(async_auto_task, &context, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Autonomous Async");
  autonomous->run(context);
  pros::c::task_delete(async_task);
  async_task = nullptr;
}
} // namespace robot::autonomous
