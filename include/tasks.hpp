#ifndef TASKS_HPP
#define TASKS_HPP
void createChildTask(const char *name, void (*function)(void *), void *param, int priority = 8,
                     int stackDepth = 0x1000);
void createTask(const char *name, void (*function)(void *), void *param, int priority = 8, int stackDepth = 0x1000);

void onRootTaskStart();
void onRootTaskEnd();
#endif // TASKS_HPP
