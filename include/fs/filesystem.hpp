#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP
#include <filesystem>
#include <fstream>

namespace fs {
bool is_connected();
std::ofstream create_sink();

bool file_exists(const char *name);

std::ofstream create(const char *name);
std::ofstream create_truncate(const char *name);
std::ofstream create_indexed(const char *name);
} // namespace fs
#endif