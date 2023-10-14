#include "filesystem.hpp"
#include "debug/logger.hpp"
#include "pros/misc.h"
#include <cstring>

namespace fs {
bool is_available() { return pros::c::usd_is_installed() != 0; }

bool can_access(const std::filesystem::path &path) {
  if (!is_available()) {
    logger::warn("MicroSD unavailable. Cannot access %s", path.c_str());
    return false;
  } else if (std::strncmp(path.c_str(), "/usd/", std::strlen("/usd/")) != 0) {
    logger::warn("Attempted to access file '%s' outside of MicroSD!", path.c_str());
    return false;
  }
  return true;
}

bool file_exists(const std::filesystem::path &path) { return can_access(path) /* && std::filesystem::exists(path)*/; }

void *read_all(const char *path) {
  if (!file_exists(path)) {
    logger::warn("File does not exist: %s", path);
    return nullptr;
  }
  size_t len = std::filesystem::file_size(path) + 1;
  void *contents = std::malloc(len);
  std::unique_ptr<std::FILE, decltype(&fclose)> file =
      std::unique_ptr<std::FILE, decltype(&fclose)>(std::fopen(path, "r"), &fclose);
  size_t read = std::fread(contents, 1, len, file.get());
  if (read != len) {
    logger::warn("File size mismatch: Expected %i bytes, found %i bytes", len, read);
    void *reallocated = realloc(contents, read);
    if (reallocated != nullptr) {
      contents = reallocated;
    }
  }

  return contents;
}

std::unique_ptr<std::ifstream> open(const std::filesystem::path &path, std::ios_base::openmode mode) {
  std::unique_ptr<std::ifstream> stream = std::make_unique<std::ifstream>();
  if (can_access(path)) {
    stream->open(path, mode);
  }

  if (!stream->is_open()) {
    logger::warn("Failed to open file %s!", path.c_str());
    stream->setstate(std::ios::badbit);
  }
  return stream;
}

std::unique_ptr<std::ofstream> open_indexed(const std::filesystem::path &path, std::ios_base::openmode mode) {
  std::unique_ptr<std::ofstream> stream = std::make_unique<std::ofstream>();
  if (can_access(path)) {
    if (file_exists(path)) {
      for (auto i = 1; i < 1000; i++) {
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
    logger::warn("Failed to initialize file '%s'!", path.c_str());
    stream->setstate(std::ios::badbit);
  }
  return stream;
}
} // namespace fs
