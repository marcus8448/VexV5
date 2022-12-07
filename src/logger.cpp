#include "logger.hpp"
#include "pros/rtos.hpp"

#include <iostream>
#include <vector>

#define DEBUG_LOG

namespace logger {
static std::vector<std::pair<const char *, uint32_t>> *sections =
    new std::vector<std::pair<const char *, uint32_t>>(); // stores the name and timestamp of sections.

void info(const char *string) { std::cout << string << std::endl; }

void info(const std::string &string) { std::cout << string << std::endl; }

void warn(const char *string) { std::cout << string << std::endl; }

void warn(const std::string &string) { std::cout << string << std::endl; }

void error(const char *string) { std::cout << string << std::endl; }

void error(const std::string &string) { std::cout << string << std::endl; }

void debug(const char *string) {
#ifdef DEBUG_LOG
  std::cout << string << std::endl;
#endif
}

void debug(const std::string &string) {
#ifdef DEBUG_LOG
  std::cout << string << std::endl;
#endif
}

void push(const char *string) {
  sections->emplace_back(std::pair(string, pros::millis()));
  std::cout << "== BEGIN " << string << " ==" << std::endl;
}

void pop_push(const char *string) {
  pop();
  push(string);
}

void pop() {
  uint32_t millis = pros::millis();
  if (sections->empty()) {
    error("Section stack underflow!");
  }
  std::pair<const char *, uint32_t> &back = sections->back();
  std::cout << "=== END " << back.first << " ==="
            << " (Took " << millis - back.second << " ms)" << std::endl;
  sections->pop_back();
}
} // namespace logger
