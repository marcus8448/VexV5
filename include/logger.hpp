#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <memory>
#include <string>

namespace logger {
/**
 * Formats a string, similar to printf, but without printing it.
 * @tparam Args The type of arguments to use.
 * @param format The format string.
 * @param args The type arguments to insert into the format string.
 * @return The formatted string.
 */
template <typename... Args> std::string string_format(const char *format, Args... args) {
  uint32_t size_s = std::snprintf(nullptr, 0, format, args...) + 1; // Extra space for '\0'
  if (size_s <= 0) {
    return "INVALID STRING";
  }
  auto size = static_cast<size_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, format, args...);
  return {buf.get(), buf.get() + size - 1}; // We don't want the '\0' inside
}

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
template <typename... Args> void info(const char *format, Args... args) { info(string_format(format, args...)); }

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
template <typename... Args> void warn(const char *format, Args... args) { warn(string_format(format, args...)); }

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
template <typename... Args> void error(const char *format, Args... args) { error(string_format(format, args...)); }

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
template <typename... Args> void debug(const char *format, Args... args) { debug(string_format(format, args...)); }

/**
 * Pushes a logging section onto the stack.
 * If the DEBUG_LOG flag is set, it will print the name to standard output
 * Used for timing and debugging purposes.
 * @param string the name of the section
 */
void push(const char *string);

/**
 * Pops a logging section from the stack.
 * If the DEBUG_LOG flag is set, it will print the name and time to standard output
 * Used for timing and debugging purposes.
 */
void pop();

/**
 * Pops a logging section from the stack, and then immediately pushes a new one.
 * If the DEBUG_LOG flag is set, it will print the names and time to standard output
 * Used for timing and debugging purposes.
 * @param string the name of the new section
 */
void pop_push(const char *string);
} // namespace logger
#endif // LOGGER_HPP