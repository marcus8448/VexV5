#ifndef DEBUG_ERROR_HPP
#define DEBUG_ERROR_HPP
#include <numeric>
#include <cstdint>

namespace error {
constexpr double INTEGER = std::numeric_limits<int32_t>::max();
constexpr double FLOATING = std::numeric_limits<double>::infinity();

void print(const char *name);

bool check(double val);
bool check(int32_t val);

bool isDisconnected();
}
#endif // DEBUG_ERROR_HPP
