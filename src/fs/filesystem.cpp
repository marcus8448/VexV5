#include "fs/filesystem.hpp"
#include "logger.hpp"
#include "pros/misc.hpp"
#include "pros/rtos.h"
#include <fstream>

namespace fs {
std::string to_path(const char *name) { return std::string("/usd/").append(name); }

bool is_connected() { return pros::usd::is_installed(); }

std::ofstream create_sink() { return std::ofstream("/dev/null"); }
std::ifstream create_read_sink() { return std::ifstream("/dev/null"); }

bool file_exists(const char *name) {
  if (!pros::usd::is_installed()) {
    warn("MicroSD unavailable!");
    return false;
  }

  if (std::FILE *file = std::fopen(name, "r")) {
    std::fclose(file);
    return true;
  } else {
    return false;
  }
}

std::ifstream open(const char *name) {
  std::string path = to_path(name);
  if (!pros::usd::is_installed()) {
    warn("MicroSD unavailable, creating sink.");
    return create_read_sink();
  }
  if (!file_exists(name)) {
    return create_read_sink();
  }
  return std::ifstream(path);
}

std::ofstream create(const char *name) {
  std::string path = to_path(name);
  if (!pros::usd::is_installed()) {
    warn("MicroSD unavailable, creating sink.");
    return create_sink();
  }
  return std::ofstream(path, std::ios_base::out | std::ios::app);
}

std::ofstream create_truncate(const char *name) {
  std::string path = to_path(name);
  if (!pros::usd::is_installed()) {
    warn("MicroSD unavailable, creating sink.");
    return create_sink();
  }
  return std::ofstream(path, std::ios_base::out | std::ios::trunc);
}

std::ofstream create_indexed(const char *name) { // todo: fails due to too many open file handles, but we close all test
                                                 // ones (so how does this occur?)
  std::string path = to_path(name);
  if (!pros::usd::is_installed()) {
    warn("MicroSD unavailable, creating sink.");
    return create_sink();
  }
  if (!file_exists(path.c_str())) {
    return std::ofstream(path);
  }
  for (int16_t i = 1; i < 1000; i++) {
    std::string numPath = std::string(path).append("_").append(std::to_string(i));
    if (!file_exists(numPath.c_str())) {
      return std::ofstream(numPath);
    }
  }
  return create_sink();
}
} // namespace fs
