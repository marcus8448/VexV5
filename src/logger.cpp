#include "logger.hpp"
#include "fs/filesystem.hpp"
#include "pros/rtos.hpp"

#include <iostream>
#include <vector>

namespace logger {
static std::vector<std::pair<const char *, uint32_t>> *sections =
    new std::vector<std::pair<const char *, uint32_t>>(); // stores the name and timestamp of sections.
static const char *main_task_name = nullptr;

#ifdef FILE_LOG
std::ofstream create_logfile(const char *name);
static std::ofstream log_file = create_logfile("log");
#endif

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
  if (main_task_name != pros::Task::current().get_name()) {
    warn("Called section push on non-main task!");
    return;
  }
  sections->emplace_back(std::pair(string, pros::millis()));
  std::cout << "== BEGIN " << string << " ==" << std::endl;
#ifdef FILE_LOG
  log_file << "[DEBUG] SECTION BEGIN: " << string << std::endl;
#endif
}

void _pop() {
  if (main_task_name != pros::Task::current().get_name()) {
    warn("Called section pop on non-main task!");
    return;
  }
  uint32_t millis = pros::millis();
  if (sections->empty()) {
    _error("Section stack underflow!");
  }
  std::pair<const char *, uint32_t> &back = sections->back();
  std::cout << "=== END " << back.first << " ==="
            << " (Took " << millis - back.second << " ms)" << std::endl;

#ifdef FILE_LOG
  log_file << "[DEBUG] [" << pros::millis() << "] SECTION END: " << back.first << " (Took " << millis - back.second
           << " ms)" << std::endl;
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

void initialize(const char *name) {
  main_task_name = name;
  if (!sections->empty()) {
    warn("Debug sections not properly cleared!");
  }
  sections->clear();
}

#ifdef FILE_LOG
std::ofstream create_logfile(const char *name) { return fs::create_append(name); }
#endif

} // namespace logger
