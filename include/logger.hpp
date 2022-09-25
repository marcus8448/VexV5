#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <string>
#include <memory>

namespace logger {
template<typename ... Args>
std::string string_format(const char *format, Args ... args) {
  uint32_t size_s = std::snprintf(nullptr, 0, format, args...) + 1; // Extra space for '\0'
  if (size_s <= 0) {
    return "INVALID STRING";
  }
  auto size = static_cast<size_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, format, args...);
  return {buf.get(), buf.get() + size - 1}; // We don't want the '\0' inside
}

void info(const char *string);
void info(const std::string &string);
template<typename ... Args> void info(const char *format, Args ... args) {
  info(string_format(format, args...));
}

void warn(const char *string);
void warn(const std::string &string);
template<typename ... Args> void warn(const char *format, Args ... args) {
  warn(string_format(format, args...));
}
void error(const char *string);
void error(const std::string &string);
template<typename ... Args> void error(const char *format, Args ... args) {
  error(string_format(format, args...));
}
void debug(const char *string);
void debug(const std::string &string);
template<typename ... Args> void debug(const char *format, Args ... args) {
  debug(string_format(format, args...));
}
void push_section(const char *string);
void pop_section();
} // namespace logger
#endif//LOGGING_HPP