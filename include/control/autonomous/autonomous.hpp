#ifndef CONTROL_AUTONOMOUS_AUTONOMOUS_HPP
#define CONTROL_AUTONOMOUS_AUTONOMOUS_HPP

#include "robot/robot.hpp"
#include <map>
#include <string>

using namespace robot;

namespace control::autonomous {
void initialize();

/**
 * Registers an autonomous instance to the system.
 * @param name The name of the autonomous run, to be shown on the screen. Must be unique.
 * @param function The autonomous run
 */
void registerRun(const std::string &name, void (*function)(robot::Robot &));

std::map<const std::string, void (*)(robot::Robot &)> &getPrograms();

/**
 * Returns the selected autonomous run.
 * @return The selected autonomous run.
 */
void run(Robot &robot);
} // namespace control::autonomous
#endif // CONTROL_AUTONOMOUS_AUTONOMOUS_HPP
