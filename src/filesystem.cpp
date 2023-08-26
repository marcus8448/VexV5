#include "filesystem.hpp"
#include "debug/logger.hpp"
#include "pros/misc.h"
#include <cstring>

namespace fs {
bool is_available() { return pros::c::usd_is_installed(); }

bool can_access(const std::filesystem::path &path) {
  if (!is_available()) {
    warn("MicroSD unavailable. Cannot access %s", path.c_str());
    return false;
  } else if (std::strncmp(path.c_str(), "/usd/", strlen("/usd/")) != 0) {
    warn("Attempted to access file '%s' outside of MicroSD!", path.c_str());
    return false;
  }
  return true;
}

bool file_exists(const std::filesystem::path &path) { return can_access(path) /* && std::filesystem::exists(path)*/; }

void *read_all(const char *path) {
  if (!file_exists(path)) {
    warn("File does not exist: %s", path);
    return nullptr;
  }
  size_t len = std::filesystem::file_size(path) + 1;
  void *contents = std::malloc(len);
  std::FILE *file = std::fopen(path, "r");
  size_t read = std::fread(contents, 1, len, file);
  if (std::fclose(file) != 0) {
    warn("Failed to lower file %s!", path);
  }
  if (read != len) {
    warn("File size mismatch: Expected %i bytes, found %i bytes", len, read);
    void *reallocated = realloc(contents, read);
    if (reallocated != nullptr) {
      contents = reallocated;
    }
  }

  return contents;
}

std::ifstream *open(const std::filesystem::path &path, std::ios_base::openmode mode) {
  auto stream = new std::ifstream();
  if (can_access(path)) {
    stream->open(path, mode);
  }

  if (!stream->is_open()) {
    warn("Failed to open file %s!", path.c_str());
    stream->setstate(std::ios::badbit);
  }
  return stream;
}

std::ofstream *open_indexed(const std::filesystem::path &path, std::ios_base::openmode mode) {
  auto *stream = new std::ofstream();
  if (can_access(path)) {
    if (file_exists(path)) {
      for (int16_t i = 1; i < 1000; i++) {
        std::string indexedPath = std::string(path) + "." + std::to_string(i);
        if (!file_exists(indexedPath)) {
          std::filesystem::rename(path, indexedPath);
          break;
        }
      }
    }
    stream->open(path, mode);
  }

  if (!stream->is_open()) {
    warn("Failed to initialize file '%s'!", path.c_str());
    stream->setstate(std::ios::badbit);
  }
  return stream;
}
} // namespace fs
