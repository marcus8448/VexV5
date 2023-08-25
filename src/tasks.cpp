#include "tasks.hpp"
#include "debug/logger.hpp"
#include "pros/rtos.h"
#include <vector>

namespace rtos {
static pros::task_t rootTask = nullptr;
static std::vector<pros::task_t> parallelRootTasks = std::vector<pros::task_t>();

pros::task_t createChildTask(const char *name, void (*function)(void *), void *param, int priority, int stackDepth) {
  info("Create child task %s", name);
  pros::task_t task = pros::c::task_create(function, param, priority, stackDepth, name);
  parallelRootTasks.emplace_back(task);
  return task;
}

pros::task_t createTask(const char *name, void (*function)(void *), void *param, int priority, int stackDepth) {
  info("Create task %s", name);
  return pros::c::task_create(function, param, priority, stackDepth, name);
}

void killTask(pros::task_t task) {
  info("Kill task %s", pros::c::task_get_name(task));
  pros::c::task_delete(task);
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
    logger::clearRoot(pros::c::task_get_name(rootTask));
    logger::flush();
  }

  for (pros::task_t task : parallelRootTasks) {
    pros::task_state_e_t state = pros::c::task_get_state(task);
    if (state != pros::E_TASK_STATE_INVALID && state != pros::E_TASK_STATE_DELETED) {
      pros::c::task_delete(task);
    }
  }
  parallelRootTasks.clear();
  rootTask = nullptr;
}

void killRootTask() {
  info("Kill root task?");
  if (rootTask != nullptr) {
    info("Kill root task %s", pros::c::task_get_name(rootTask));
    void* task = rootTask;
    onRootTaskEnd();
    pros::task_state_e_t state = pros::c::task_get_state(task);
    if (state != pros::E_TASK_STATE_INVALID && state != pros::E_TASK_STATE_DELETED) {
      pros::c::task_delete(task);
    }
  }
}
} // namespace rtos
