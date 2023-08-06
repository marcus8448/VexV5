#include "tasks.hpp"
#include "debug/logger.hpp"
#include "pros/rtos.h"
#include <vector>

static pros::task_t rootTask = nullptr;
static std::vector<pros::task_t> *parallelRootTasks = new std::vector<pros::task_t>();

void createChildTask(const char *name, void (*function)(void *), void *param, int priority, int stackDepth) {
  pros::task_t task = pros::c::task_create(function, param, priority, stackDepth, name);
  parallelRootTasks->emplace_back(task);
}

void createTask(const char *name, void (*function)(void *), void *param, int priority, int stackDepth) {
  pros::c::task_create(function, param, priority, stackDepth, name);
}

void onRootTaskStart() {
  if (rootTask != nullptr) {
    onRootTaskEnd();
  }
  rootTask = pros::c::task_get_current();
  logger::flush();
  logger::initialize(pros::c::task_get_name(rootTask));
}

void onRootTaskEnd() {
  if (rootTask != nullptr) {
    logger::clearRoot(pros::c::task_get_name(pros::c::task_get_current()));
    logger::flush();
  }

  for (pros::task_t task : *parallelRootTasks) {
    pros::c::task_delete(task);
  }
  parallelRootTasks->clear();
  rootTask = nullptr;
}
