#ifndef ROBOT_DEVICE_DEVICE_HPP
#define ROBOT_DEVICE_DEVICE_HPP

#include <cstdint>

namespace robot::device {
class Device {
public:
  explicit Device(uint8_t port);
  virtual void reconfigure() const = 0;
  [[nodiscard]] virtual bool is_connected() const = 0;
  [[nodiscard]] uint8_t get_port() const;

protected:
  const uint8_t port;

  bool checkConnect() const;
};
} // namespace robot::device
#endif // ROBOT_DEVICE_DEVICE_HPP
