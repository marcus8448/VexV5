#include "config.hpp"
#include "logger.hpp"
#include <iostream>
#include <vector>

namespace logger {
static std::vector<const char *> SECTIONS;

void info(const char *string) {
  std::cout << string;
}

void info(const std::string string) {
  std::cout << string;
}

void warn(const char *string) {
  std::cout << string;
}

void warn(const std::string string) {
  std::cout << string;
}

void error(const char *string) {
  std::cout << string; //todo: cerr?
}

void error(const std::string string) {
  std::cout << string;
}

void debug(const char *string) {
#ifdef DEBUG_LOG
  std::cout << string;
#endif
}

void debug(const std::string string) {
#ifdef DEBUG_LOG
  std::cout << string;
#endif
}

void push_section(const char *string) {
#ifdef DEBUG_LOG
  SECTIONS.push_back(string);
  std::cout << "== BEGIN " << string << " ==";
#endif
}

void pop_section() {
#ifdef DEBUG_LOG
  if (SECTIONS.empty()) {
      error("Section stack underflow!");
  }
  std::cout << "=== END " << SECTIONS.back() << " ===";
  SECTIONS.pop_back();
#endif
}
} //namespace logger
