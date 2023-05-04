#include "include/debug/logger.hpp"
#include "fs/filesystem.hpp"
#include "pros/rtos.hpp"

#include <iostream>
#include <vector>

#define FILE_LOG

namespace logger {
static std::vector<std::pair<const char *, uint32_t>> *sections =
    new std::vector<std::pair<const char *, uint32_t>>(); // stores the name and timestamp of sections.
static const char *main_task_name = nullptr;

#ifdef FILE_LOG
static std::ofstream* log_file = nullptr;
#endif

void _info(const char *string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open())
    *log_file << "[INFO] [" << pros::millis() << "] " << string << std::endl;
#endif
}

void _info(const std::string &string) {
  _info(string.c_str());
}

void _warn(const char *string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open())
    *log_file << "[WARN] [" << pros::millis() << "] " << std::endl;
#endif
}

void _warn(const std::string &string) {
  _warn(string.c_str());
}

void _error(const char *string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open())
    *log_file << "[ERROR] [" << pros::millis() << "] " << std::endl;
#endif
}

void _error(const std::string &string) {
  _error(string.c_str());
}

#ifdef DEBUG_LOG
void _debug(const char *string) {
  std::cout << string << std::endl;
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open())
    *log_file << "[DEBUG] [" << pros::millis() << "] " << std::endl;
#endif
}

void _debug(const std::string &string) {
  _debug(string.c_str());
}

void _push(const char *string) {
  if (main_task_name != pros::Task::current().get_name()) {
    _error("Called section push on non-main task!");
    return;
  }
  sections->emplace_back(std::pair(string, pros::millis()));
  debug("== BEGIN %s ==", string);
}

void _pop() {
  if (main_task_name != pros::Task::current().get_name()) {
    _error("Called section pop on non-main task!");
    return;
  }
  uint32_t millis = pros::millis();
  if (sections->empty()) {
    _error("Section stack underflow!");
  } else {
    std::pair<const char *, uint32_t> &back = sections->back();
    debug("== END %s [%ims]==", back.first, millis - back.second);
    sections->pop_back();
  }
}

void _pop_push(const char *string) {
  _pop();
  _push(string);
}

#endif // DEBUG_LOG

void flush() {
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    log_file->flush();
  }
#endif
}

void initialize(const char *name) {
  main_task_name = name;
  if (!sections->empty()) {
    warn("Debug sections not properly cleared!");
  }
  sections->clear();

#ifdef FILE_LOG
  if (log_file == nullptr) {
    auto* stream = static_cast<std::ofstream *>(malloc(sizeof(std::ofstream)));
    *stream = fs::open_indexed("log");

    if (stream->is_open()) {
      log_file = stream;
    } else {
      stream->close();
      free(stream);
    }
  }
#endif
}
} // namespace logger
