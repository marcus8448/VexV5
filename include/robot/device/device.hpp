#ifndef ROBOT_DEVICE_DEVICE_HPP
#define ROBOT_DEVICE_DEVICE_HPP

#include <cstdint>

namespace robot::device {
class Device {
public:
  [[nodiscard]] virtual bool is_connected() = 0;
  [[nodiscard]] virtual uint8_t get_port() const = 0;
};
} // namespace robot::device
#endif // ROBOT_DEVICE_DEVICE_HPP
