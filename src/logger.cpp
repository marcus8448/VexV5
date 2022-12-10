#include "logger.hpp"
#include "fs/filesystem.hpp"
#include "pros/rtos.hpp"

#include <iostream>
#include <vector>

#define DEBUG_LOG
// #define FILE_LOG

namespace logger {
static std::vector<std::pair<const char *, uint32_t>> *sections =
    new std::vector<std::pair<const char *, uint32_t>>(); // stores the name and timestamp of sections.

#ifdef FILE_LOG
static std::ofstream log_file = fs::create_indexed("log");
#endif

void info(const char *string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[INFO] [" << pros::millis() << "] " << string << std::endl;
#endif
}

void info(const std::string &string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[INFO] [" << pros::millis() << "] " << std::endl;
#endif
}

void warn(const char *string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[WARN] [" << pros::millis() << "] " << std::endl;
#endif
}

void warn(const std::string &string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[WARN] [" << pros::millis() << "] " << std::endl;
#endif
}

void error(const char *string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[ERROR] [" << pros::millis() << "] " << std::endl;
#endif
}

void error(const std::string &string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[ERROR] [" << pros::millis() << "] " << std::endl;
#endif
}

void debug(const char *string) {
#ifdef DEBUG_LOG
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[DEBUG] [" << pros::millis() << "] " << std::endl;
#endif
#endif
}

void debug(const std::string &string) {
#ifdef DEBUG_LOG
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[DEBUG] [" << pros::millis() << "] " << std::endl;
#endif
#endif
}

void push(const char *string) {
  sections->emplace_back(std::pair(string, pros::millis()));
  std::cout << "== BEGIN " << string << " ==" << std::endl;
#ifdef FILE_LOG
  log_file << "[DEBUG] SECTION BEGIN: " << string << std::endl;
#endif
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

#ifdef FILE_LOG
  log_file << "[DEBUG] SECTION END: " << back.first << " (Took " << millis - back.second << " ms)" << std::endl;
#endif
  sections->pop_back();
}
} // namespace logger
