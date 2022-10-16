#include "logger.hpp"
#include <iostream>
#include <vector>

#include "screen/colour.hpp"
#include "screen/logging.hpp"

#define DEBUG_LOG

namespace logger {
static std::vector<std::pair<const char *, uint32_t>> sections; // stores the name and timestamp of sections.

void info(const char *string) {
  if (screen::logging != nullptr) {
    screen::logging->write_line(string, screen::colour::WHITE);
  }
  std::cout << string << std::endl;
}

void info(const std::string &string) {
  if (screen::logging != nullptr) {
    screen::logging->write_line(string, screen::colour::WHITE);
  }
  std::cout << string << std::endl;
}

void warn(const char *string) {
  if (screen::logging != nullptr) {
    screen::logging->write_line(string, screen::colour::YELLOW);
  }
  std::cout << string << std::endl;
}

void warn(const std::string &string) {
  if (screen::logging != nullptr) {
    screen::logging->write_line(string, screen::colour::YELLOW);
  }
  std::cout << string << std::endl;
}

void error(const char *string) {
  if (screen::logging != nullptr) {
    screen::logging->write_line(string, screen::colour::RED);
  }
  std::cout << string << std::endl;
}

void error(const std::string &string) {
  if (screen::logging != nullptr) {
    screen::logging->write_line(string, screen::colour::RED);
  }
  std::cout << string << std::endl;
}

void debug(const char *string) {
#ifdef DEBUG_LOG
  if (screen::logging != nullptr) {
    screen::logging->write_line(string, screen::colour::GREEN);
  }
  std::cout << string << std::endl;
#endif
}

void debug(const std::string &string) {
#ifdef DEBUG_LOG
  if (screen::logging != nullptr) {
    screen::logging->write_line(string, screen::colour::GREEN);
  }
  std::cout << string << std::endl;
#endif
}

void push(const char *string) {
#ifdef DEBUG_LOG
  sections.emplace_back(std::pair(string, pros::millis()));
  std::cout << "== BEGIN " << string << " ==" << std::endl;
#endif
}

void pop_push(const char *string) {
#ifdef DEBUG_LOG
  pop();
  push(string);
#endif
}

void pop() {
#ifdef DEBUG_LOG
  uint32_t millis = pros::millis();
  if (sections.empty()) {
    error("Section stack underflow!");
  }
  std::pair<const char *, uint32_t> &back = sections.back();
  std::cout << "=== END " << back.first << " ==="
            << " (Took " << millis - back.second << " ms)" << std::endl;
  sections.pop_back();
#endif
}
} // namespace logger
