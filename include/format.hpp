#ifndef FORMAT_HPP
#define FORMAT_HPP
#include <cstring>
#include <format>
#include <string>

namespace fmt {
/**
 * Formats a string, similar to printf, but without printing it.
 * @tparam Args The type of arguments to use.
 * @param format The format string.
 * @param args The type arguments to insert into the format string.
 * @return The formatted string.
 */
template <typename... Args> std::string string_format(std::format_string<Args...> format, Args &&...args) {
  return std::vformat(format.get(), std::make_format_args(args...));
}
/**
 * Formats a string, similar to printf, but without printing it.
 * @tparam Args The type of arguments to use.
 * @param format The format string.
 * @param args The type arguments to insert into the format string.
 * @return The formatted string.
 */
template <typename... Args> const char *leak_string_format(const char *format, Args... args) {
  const std::string str = std::vformat(format, std::make_format_args(args...));
  char *out = new char[str.length() + 1];
  str.copy(out, str.length());
  out[str.length()] = '\0';
  return out;
}
} // namespace fmt

#endif // FORMAT_HPP
