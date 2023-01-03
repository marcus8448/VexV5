#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <memory>
#include <string>

#define DEBUG_LOG
// #define FILE_LOG

#define info(fmt, ...) logger::_info(fmt, ##__VA_ARGS__)
#define warn(fmt, ...) logger::_warn(fmt, ##__VA_ARGS__)
#define error(fmt, ...) logger::_error(fmt, ##__VA_ARGS__)

#ifdef DEBUG_LOG
#define debug(fmt, ...) logger::_debug(fmt, ##__VA_ARGS__)
#define section_push(name) logger::_push(name)
#define section_pop() logger::_pop()
#define section_swap(name) logger::_pop_push(name)
#else
#define debug(fmt, ...) {}
#define section_push(name) {}
#define section_pop() {}
#define section_swap(name) {}
#endif

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
void _info(const char *string);

/**
 * Prints out a message via standard output.
 * May also be printed to the robot screen with white text colour.
 * @param string The string to print out.
 */
void _info(const std::string &string);

/**
 * Prints out a formatted message via standard output.
 * May also be printed to the robot screen with white text colour.
 * @param string The string to print out.
 * @see string_format
 */
template <typename... Args> void _info(const char *format, Args... args) { _info(string_format(format, args...)); }

/**
 * Prints out a message via standard output.
 * May also be printed to the robot screen with yellow text colour.
 * @param string The string to print out.
 */
void _warn(const char *string);

/**
 * Prints out a message via standard output.
 * May also be printed to the robot screen with yellow text colour.
 * @param string The string to print out.
 */
void _warn(const std::string &string);

/**
 * Prints out a formatted message via standard output.
 * May also be printed to the robot screen with yellow text colour.
 * @param string The string to print out.
 * @see string_format
 */
template <typename... Args> void _warn(const char *format, Args... args) { _warn(string_format(format, args...)); }

/**
 * Prints out a message via standard output.
 * May also be printed to the robot screen with red text colour.
 * @param string The string to print out.
 */
void _error(const char *string);

/**
 * Prints out a message via standard output.
 * May also be printed to the robot screen with red text colour.
 * @param string The string to print out.
 */
void _error(const std::string &string);

/**
 * Prints out a formatted message via standard output.
 * May also be printed to the robot screen with red text colour.
 * @param string The string to print out.
 * @see string_format
 */
template <typename... Args> void _error(const char *format, Args... args) { _error(string_format(format, args...)); }

#ifdef DEBUG_LOG
/**
 * Prints out a message via standard output, if the DEBUG_LOG flag is set.
 * May also be printed to the robot screen with green text colour.
 * @param string The string to print out.
 */
void _debug(const char *string);

/**
 * Prints out a message via standard output, if the DEBUG_LOG flag is set.
 * May also be printed to the robot screen with green text colour.
 * @param string The string to print out.
 */
void _debug(const std::string &string);

/**
 * Prints out a formatted message via standard output, if the DEBUG_LOG flag is set.
 * May also be printed to the robot screen with green text colour.
 * @param string The string to print out.
 * @see string_format
 */
template <typename... Args> void _debug(const char *format, Args... args) { _debug(string_format(format, args...)); }

/**
 * Pushes a logging section onto the stack.
 * If the DEBUG_LOG flag is set, it will print the name to standard output
 * Used for timing and debugging purposes.
 * @param string the name of the section
 */
void _push(const char *string);

/**
 * Pops a logging section from the stack.
 * If the DEBUG_LOG flag is set, it will print the name and time to standard output
 * Used for timing and debugging purposes.
 */
void _pop();

/**
 * Pops a logging section from the stack, and then immediately pushes a new one.
 * If the DEBUG_LOG flag is set, it will print the names and time to standard output
 * Used for timing and debugging purposes.
 * @param string the name of the new section
 */
void _pop_push(const char *string);
#endif // DEBUG_LOG

void flush();

} // namespace logger
#endif // LOGGER_HPP
