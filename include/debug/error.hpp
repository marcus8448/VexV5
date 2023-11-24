#ifndef DEBUG_ERROR_HPP
#define DEBUG_ERROR_HPP
#include <cstdint>
#include <numeric>
#include <string_view>

namespace error {
constexpr int32_t INTEGER = std::numeric_limits<int32_t>::max();
constexpr float FLOATING = std::numeric_limits<float>::infinity();
constexpr double DOUBLE = std::numeric_limits<double>::infinity();

void print(std::string_view name);

bool check(double val);
bool check(int32_t val);

bool isDisconnected();
} // namespace error
#endif // DEBUG_ERROR_HPP
