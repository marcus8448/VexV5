#include "logger.hpp"
#include "pros/rtos.h"

#include <cstring>
#include <iostream>
#include <unordered_map>
#include <vector>

#ifdef FILE_LOG
#include "filesystem.hpp"
#endif

namespace logger {
static std::unordered_map<pros::task_t, std::vector<std::pair<const char *, uint32_t>>> sections =
    std::unordered_map<pros::task_t,
                       std::vector<std::pair<const char *, uint32_t>>>(); // stores the name and timestamp of sections.

static const char *main_task_name = nullptr;

#ifdef FILE_LOG
static std::unique_ptr<std::ofstream> log_file = nullptr;
#endif

void info(const char *string) {
  puts(string);
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[INFO] [" << pros::c::millis() << "] " << string << '\n';
    log_file->flush();
  }
#endif
}

void info(std::string_view string) {
  std::cout << string << '\n';
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[INFO] [" << pros::c::millis() << "] " << string << '\n';
    log_file->flush();
  }
#endif
  std::cout.flush();
}

void warn(const char *string) {
  puts(string);
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[WARN] [" << pros::c::millis() << "] " << '\n';
    log_file->flush();
  }
#endif
}

void warn(std::string_view string) {
  std::cout << string << '\n';
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[WARN] [" << pros::c::millis() << "] " << '\n';
    log_file->flush();
  }
#endif
  std::cout.flush();
}

void error(const char *string) {
  puts(string);
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[ERROR] [" << pros::c::millis() << "] " << '\n';
    log_file->flush();
  }
#endif
}

void error(std::string_view string) {
  std::cout << string << '\n';
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[ERROR] [" << pros::c::millis() << "] " << '\n';
    log_file->flush();
  }
#endif
  std::cout.flush();
}

#ifdef DEBUG_LOG
void debug(const char *string) {
  puts(string);
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[DEBUG] [" << pros::c::millis() << "] " << '\n';
    log_file->flush();
  }
#endif
}

void debug(std::string_view string) {
  std::cout << string << '\n';
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    *log_file << "[DEBUG] [" << pros::c::millis() << "] " << '\n';
    log_file->flush();
  }
#endif
  std::cout.flush();
}
#else
void debug(const char *) {}

void debug(std::string_view) {}
#endif // DEBUG_LOG

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
  info("> START {}", str.c_str());
}

void endScope() {
  pros::task_t task = pros::c::task_get_current();
  std::vector<std::pair<const char *, uint32_t>> &stack = sections.at(task);
  uint32_t millis = pros::c::millis();
  if (stack.empty()) {
    error("Section stack underflow!");
  } else {
    auto &[name, start] = stack.back();
    std::string str;
    for (const auto &[name, time] : stack) {
      str.append("/").append(name);
    }
    info("> END {} [{}ms]", str.c_str(), millis - start);
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

void flush() {
#ifdef FILE_LOG
  if (log_file != nullptr && log_file->is_open()) {
    log_file->flush();
  }
#endif
}

void initialize(const char *name) {
  main_task_name = name;
  info("Root task '{}' started.", main_task_name);

#ifdef FILE_LOG
  if (log_file == nullptr) {

    if (auto stream = fs::open_indexed("log"); stream != nullptr) {
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
      warn("Clearing root task ({}) after other task ({}) has started!", name, main_task_name);
    } else {
      info("Root task '{}' ended.", name);
    }
  } else {
    warn("Clearing root task ({}) with no task active!", name);
  }
  main_task_name = nullptr;
}
} // namespace logger
