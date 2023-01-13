#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP
#include <filesystem>
#include <fstream>

namespace fs {
bool is_connected();
std::ofstream create_sink();
std::ifstream create_read_sink();

bool file_exists(const char *name);

std::ifstream open(const char *name);

std::ofstream create_append(const char *name);
std::ofstream create_truncate(const char *name);
std::ofstream create_binary(const char *name);
std::ofstream create_indexed(const char *name);
} // namespace fs
#endif
