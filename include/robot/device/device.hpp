#ifndef ROBOT_DEVICE_DEVICE_HPP
#define ROBOT_DEVICE_DEVICE_HPP

#define DEVICE_NAME(name)                                                                                              \
  [[nodiscard]] const char *device_name() const override { return name; };

#include <cstdint>

namespace robot::device {
class Device {
public:
  explicit Device(uint8_t port);
  explicit Device(const Device &) = delete;

  virtual void reconfigure() const = 0;

  [[nodiscard]] virtual bool is_connected() const = 0;
  [[nodiscard]] uint8_t get_port() const;

  [[nodiscard]] virtual const char *device_name() const = 0;

protected:
  const uint8_t port;

  [[nodiscard]] static bool checkConnect();
};

void initialize();
} // namespace robot::device
#endif // ROBOT_DEVICE_DEVICE_HPP
