#ifndef DEBUG_LOGGER_HPP
#define DEBUG_LOGGER_HPP

#include "format.hpp"
#include <memory>
#include <string>

#ifdef ENABLE_TIMERS
#include "pros/rtos.h"
#define startTiming(name) auto __timer_##name##__ = pros::c::millis()
#define endTiming(name) logger::info("%s took %i ms", #name, pros::c::millis() - __timer_##name##__)
#else
#define startTiming(name)                                                                                              \
  {}
#define endTiming(name)                                                                                                \
  {}
#endif

namespace logger {

/**
 * Prints out a message via standard output.
 * May also be printed to the robot screen with white text colour.
 * @param string The string to print out.
 */
void info(const char *string);

/**
 * Prints out a message via standard output.
 * May also be printed to the robot screen with white text colour.
 * @param string The string to print out.
 */
void info(const std::string &string);

/**
 * Prints out a formatted message via standard output.
 * May also be printed to the robot screen with white text colour.
 * @param string The string to print out.
 * @see string_format
 */
template <typename... Args> void info(const char *format, Args... args) {
  logger::info(fmt::string_format(format, args...));
}

/**
 * Prints out a message via standard output.
 * May also be printed to the robot screen with yellow text colour.
 * @param string The string to print out.
 */
void warn(const char *string);

/**
 * Prints out a message via standard output.
 * May also be printed to the robot screen with yellow text colour.
 * @param string The string to print out.
 */
void warn(const std::string &string);

/**
 * Prints out a formatted message via standard output.
 * May also be printed to the robot screen with yellow text colour.
 * @param string The string to print out.
 * @see string_format
 */
template <typename... Args> void warn(const char *format, Args... args) { warn(fmt::string_format(format, args...)); }

/**
 * Prints out a message via standard output.
 * May also be printed to the robot screen with red text colour.
 * @param string The string to print out.
 */
void error(const char *string);

/**
 * Prints out a message via standard output.
 * May also be printed to the robot screen with red text colour.
 * @param string The string to print out.
 */
void error(const std::string &string);

/**
 * Prints out a formatted message via standard output.
 * May also be printed to the robot screen with red text colour.
 * @param string The string to print out.
 * @see string_format
 */
template <typename... Args> void error(const char *format, Args... args) {
  logger::error(fmt::string_format(format, args...));
}

/**
 * Prints out a message via standard output, if the DEBUG_LOG flag is set.
 * May also be printed to the robot screen with green text colour.
 * @param string The string to print out.
 */
void debug(const char *string);

/**
 * Prints out a message via standard output, if the DEBUG_LOG flag is set.
 * May also be printed to the robot screen with green text colour.
 * @param string The string to print out.
 */
void debug(const std::string &string);

/**
 * Prints out a formatted message via standard output, if the DEBUG_LOG flag is set.
 * May also be printed to the robot screen with green text colour.
 * @param string The string to print out.
 * @see string_format
 */
#ifdef DEBUG_LOG
template <typename... Args> void debug(const char *format, Args... args) { debug(fmt::string_format(format, args...)); }
#else
template <typename... Args> void debug(const char *, Args...) {
  //  debug(fmt::string_format(format, args...));
}
#endif

/**
 * Pushes a logging section onto the stack.
 * If the DEBUG_LOG flag is set, it will print the name to standard output
 * Used for timing and debugging purposes.
 * @param string the name of the section
 */
void scope(const char *string);

/**
 * Pops a logging section from the stack.
 * If the DEBUG_LOG flag is set, it will print the name and time to standard output
 * Used for timing and debugging purposes.
 */
void endScope();

/**
 * Pops a logging section from the stack, and then immediately pushes a new one.
 * If the DEBUG_LOG flag is set, it will print the names and time to standard output
 * Used for timing and debugging purposes.
 * @param string the name of the new section
 */
void swapScope(const char *string);

void flush();

void initialize(const char *name);
void clearRoot(char *name);
} // namespace logger
#endif // DEBUG_LOGGER_HPP
