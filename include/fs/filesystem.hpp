#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP
#include <filesystem>
#include <fstream>

namespace fs {
bool is_available();

bool file_exists(const std::filesystem::path &path);
bool can_access(const std::filesystem::path &path);

void *read_all(const char *path);

std::ifstream open(const std::filesystem::path &path, std::ios_base::openmode mode = std::ios::in);
std::ofstream open_indexed(const std::filesystem::path &path, std::ios_base::openmode mode = std::ios::out);
} // namespace fs
#endif
