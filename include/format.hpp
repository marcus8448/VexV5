#ifndef FORMAT_HPP
#define FORMAT_HPP
#include <memory>
#include <string>

namespace fmt {
/**
 * Formats a string, similar to printf, but without printing it.
 * @tparam Args The type of arguments to use.
 * @param format The format string.
 * @param args The type arguments to insert into the format string.
 * @return The formatted string.
 */
template <typename... Args> std::string string_format(const char *format, Args... args) {
  uint32_t size_s = snprintf(nullptr, 0, format, args...) + 1; // Extra space for '\0'
  if (size_s <= 0) {
    return "format failure";
  }
  auto size = static_cast<uint32_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format, args...);
  return {buf.get(), buf.get() + size - 1}; // We don't want the '\0' inside
}
} // namespace fmt

#endif // FORMAT_HPP
