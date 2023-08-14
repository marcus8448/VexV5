#ifndef TASKS_HPP
#define TASKS_HPP
#include "pros/rtos.h"

namespace rtos {
pros::task_t createChildTask(const char *name, void (*function)(void *), void *param, int priority = 8,
                             int stackDepth = 0x1000);
pros::task_t createTask(const char *name, void (*function)(void *), void *param, int priority = 8,
                        int stackDepth = 0x1000);
void killTask(pros::task_t task);
void onRootTaskStart();
void onRootTaskEnd();
} // namespace rtos

#endif // TASKS_HPP
