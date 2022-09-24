#include "pros/rtos.hpp"
#include "logger.hpp"
#include "robot.hpp"

void reset_positions(Robot *robot) {
  logger::push_section("Reset positions");
  robot->controller->set_line(0, 0, "Reset...");

  for (uint8_t i = 0; i < 2; i += pros::Task::notify_take(true, 0xffffffff)) {
  }

  robot->reset();
  robot->controller->set_line(0, 0, "Done!");
  logger::pop_section();
  exit(0);
}
