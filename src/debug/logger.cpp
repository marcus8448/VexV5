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

void info(const char *string) {
  std::cout << string << '\n';
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[INFO] [" << pros::c::millis() << "] " << string << '\n';
    log_file->flush();
  }
#endif
  std::cout.flush();
}

void info(const std::string &string) { info(string.c_str()); }

void warn(const char *string) {
  std::cout << string << '\n';
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[WARN] [" << pros::c::millis() << "] " << '\n';
    log_file->flush();
  }
#endif
  std::cout.flush();
}

void warn(const std::string &string) { warn(string.c_str()); }

void error(const char *string) {
  std::cout << string << '\n';
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[ERROR] [" << pros::c::millis() << "] " << '\n';
    log_file->flush();
  }
#endif
  std::cout.flush();
}

void error(const std::string &string) { error(string.c_str()); }

#ifdef DEBUG_LOG
void debug(const char *string) {
  std::cout << string << '\n';
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[DEBUG] [" << pros::c::millis() << "] " << '\n';
    log_file->flush();
  }
#endif
  std::cout.flush();
}

void debug(const std::string &string) { debug(string.c_str()); }

void scope(const char *string) {
  pros::task_t task = pros::c::task_get_current();
  if (!sections.contains(task)) {
    sections.emplace(task, std::vector<std::pair<const char *, uint32_t>>());
  }
  std::vector<std::pair<const char *, uint32_t>> &val = sections.at(task);
  val.emplace_back(string, pros::c::millis());
  std::string str;
  for (const auto &pair : val) {
    str.append("/").append(pair.first);
  }
  debug("> START %s", str.c_str());
}

void endScope() {
  pros::task_t task = pros::c::task_get_current();
  auto stack = sections.at(task);
  uint32_t millis = pros::c::millis();
  if (stack.empty()) {
    error("Section stack underflow!");
  } else {
    std::pair<const char *, uint32_t> &back = stack.back();
    std::string str;
    for (const auto &pair : stack) {
      str.append("/").append(pair.first);
    }
    debug("> END %s [%ims]", str.c_str(), millis - back.second);
    stack.pop_back();
    if (stack.empty()) {
      sections.erase(task);
    }
  }
}

void swapScope(const char *string) {
  endScope();
  scope(string);
}
#else
void debug(const char *) {}

void debug(const std::string &) {}

void scope(const char *) {}

void endScope() {}

void swapScope(const char *) {}
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
