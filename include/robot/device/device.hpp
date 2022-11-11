#include <cstdint>

namespace robot::device {
class Device {
public:
  [[nodiscard]] virtual bool is_connected() = 0;
  [[nodiscard]] virtual uint8_t get_port() const = 0;
};
} // namespace robot::device
