#include "config.hpp"
#include "logger.hpp"
#include <iostream>
#include <vector>

#ifdef SCREEN
#include "screen.hpp"
#endif

namespace logger {
static std::vector<const char *> SECTIONS;

void info(const char *string) {
#ifdef SCREEN
  screen::write_line(string, screen::WHITE);
#endif
  std::cout << string << std::endl;
}

void info(const std::string& string) {
#ifdef SCREEN
  screen::write_line(string, screen::WHITE);
#endif
  std::cout << string << std::endl;
}

void warn(const char *string) {
#ifdef SCREEN
  screen::write_line(string, screen::YELLOW);
#endif
  std::cout << string << std::endl;
}

void warn(const std::string& string) {
#ifdef SCREEN
  screen::write_line(string, screen::YELLOW);
#endif
  std::cout << string << std::endl;
}

void error(const char *string) {
#ifdef SCREEN
  screen::write_line(string, screen::RED);
#endif
  std::cout << string << std::endl;
}

void error(const std::string& string) {
#ifdef SCREEN
  screen::write_line(string, screen::RED);
#endif
  std::cout << string << std::endl;
}

void debug(const char *string) {
#ifdef DEBUG_LOG
#ifdef SCREEN
  screen::write_line(string, screen::GREEN);
#endif
  std::cout << string << std::endl;
#endif
}

void debug(const std::string& string) {
#ifdef DEBUG_LOG
#ifdef SCREEN
  screen::write_line(string, screen::GREEN);
#endif
  std::cout << string << std::endl;
#endif
}

void push_section(const char *string) {
#ifdef DEBUG_LOG
  SECTIONS.push_back(string);
  std::cout << "== BEGIN " << string << " ==" << std::endl;
#endif
}

void pop_section() {
#ifdef DEBUG_LOG
  if (SECTIONS.empty()) {
      error("Section stack underflow!");
  }
  std::cout << "=== END " << SECTIONS.back() << " ===" << std::endl;
  SECTIONS.pop_back();
#endif
}
} //namespace logger
