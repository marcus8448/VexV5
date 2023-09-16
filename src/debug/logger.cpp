#include "debug/logger.hpp"
#include "filesystem.hpp"
#include "pros/rtos.h"

#include <cstring>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace logger {
static std::unordered_map<pros::task_t, std::vector<std::pair<const char *, uint32_t>>> sections =
    std::unordered_map<pros::task_t,
                       std::vector<std::pair<const char *, uint32_t>>>(); // stores the name and timestamp of sections.
static const char *main_task_name = nullptr;

#ifdef FILE_LOG
static std::unique_ptr<std::ofstream> log_file = nullptr;
#endif

void _info(const char *string) {
  std::cout << string << '\n';
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[INFO] [" << pros::c::millis() << "] " << string << '\n';
    log_file->flush();
  }
#endif
  std::cout.flush();
}

void _info(const std::string &string) { _info(string.c_str()); }

void _warn(const char *string) {
  std::cout << string << '\n';
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[WARN] [" << pros::c::millis() << "] " << '\n';
    log_file->flush();
  }
#endif
  std::cout.flush();
}

void _warn(const std::string &string) { _warn(string.c_str()); }

void _error(const char *string) {
  std::cout << string << '\n';
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[ERROR] [" << pros::c::millis() << "] " << '\n';
    log_file->flush();
  }
#endif
  std::cout.flush();
}

void _error(const std::string &string) { _error(string.c_str()); }

#ifdef DEBUG_LOG
void _debug(const char *string) {
  std::cout << string << '\n';
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[DEBUG] [" << pros::c::millis() << "] " << '\n';
    log_file->flush();
  }
#endif
  std::cout.flush();
}

void _debug(const std::string &string) { _debug(string.c_str()); }

void _push(const char *string) {
  pros::task_t task = pros::c::task_get_current();
  if (!sections.contains(task)) {
    sections.emplace(task, std::vector<std::pair<const char *, uint32_t>>());
  }
  sections.at(task).emplace_back(string, pros::c::millis());
  debug("== BEGIN %s ==", string);
}

void _pop() {
  pros::task_t task = pros::c::task_get_current();
  auto stack = sections.at(task);
  uint32_t millis = pros::c::millis();
  if (stack.empty()) {
    _error("Section stack underflow!");
  } else {
    std::pair<const char *, uint32_t> &back = stack.back();
    debug("== END %s [%ims] ==", back.first, millis - back.second);
    stack.pop_back();
    if (stack.empty()) {
      sections.erase(task);
    }
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
  info("Root task '%s' started.", main_task_name);

#ifdef FILE_LOG
  if (log_file == nullptr) {
    auto stream = fs::open_indexed("log");

    if (stream != nullptr) {
      if (stream->is_open()) {
        log_file.swap(stream);
      } else {
        log_file.reset(nullptr);
      }
      stream.reset(nullptr);
    } else {
      log_file.reset(nullptr);
    }
  }
#endif
}

void clearRoot(char *name) {
  if (main_task_name != nullptr) {
    if (std::strcmp(name, main_task_name) != 0) {
      warn("Clearing root task (%s) after other task (%s) has started!", name, main_task_name);
    } else {
      info("Root task '%s' ended.", name);
    }
  } else {
    warn("Clearing root task (%s) with no task active!", name);
  }
  main_task_name = nullptr;
}
} // namespace logger
