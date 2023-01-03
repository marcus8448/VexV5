#include "logger.hpp"
#include "fs/filesystem.hpp"
#include "pros/rtos.hpp"

#include <iostream>
#include <vector>

namespace logger {
static std::vector<std::pair<const char *, uint32_t>> *sections =
    new std::vector<std::pair<const char *, uint32_t>>(); // stores the name and timestamp of sections.

//#ifdef FILE_LOG
static std::ofstream log_file = fs::create_indexed("log");
//#endif

void _info(const char *string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[INFO] [" << pros::millis() << "] " << string << std::endl;
#endif
}

void _info(const std::string &string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[INFO] [" << pros::millis() << "] " << std::endl;
#endif
}

void _warn(const char *string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[WARN] [" << pros::millis() << "] " << std::endl;
#endif
}

void _warn(const std::string &string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[WARN] [" << pros::millis() << "] " << std::endl;
#endif
}

void _error(const char *string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[ERROR] [" << pros::millis() << "] " << std::endl;
#endif
}

void _error(const std::string &string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[ERROR] [" << pros::millis() << "] " << std::endl;
#endif
}

#ifdef DEBUG_LOG
void _debug(const char *string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[DEBUG] [" << pros::millis() << "] " << std::endl;
#endif
}

void _debug(const std::string &string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  log_file << "[DEBUG] [" << pros::millis() << "] " << std::endl;
#endif
}

void _push(const char *string) {
  sections->emplace_back(std::pair(string, pros::millis()));
  std::cout << "== BEGIN " << string << " ==" << std::endl;
#ifdef FILE_LOG
  log_file << "[DEBUG] SECTION BEGIN: " << string << std::endl;
#endif
}

void _pop() {
  uint32_t millis = pros::millis();
  if (sections->empty()) {
    _error("Section stack underflow!");
  }
  std::pair<const char *, uint32_t> &back = sections->back();
  std::cout << "=== END " << back.first << " ==="
            << " (Took " << millis - back.second << " ms)" << std::endl;

#ifdef FILE_LOG
  log_file << "[DEBUG] [" << pros::millis() << "] SECTION END: " << back.first << " (Took " << millis - back.second << " ms)" << std::endl;
#endif
  sections->pop_back();
}

void _pop_push(const char *string) {
  _pop();
  _push(string);
}

#endif // DEBUG_LOG

void flush() {
#ifdef FILE_LOG
  log_file.flush();
#endif
}
} // namespace logger
