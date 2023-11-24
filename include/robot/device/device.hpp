#ifndef ROBOT_DEVICE_DEVICE_HPP
#define ROBOT_DEVICE_DEVICE_HPP

#include <cstdint>
#include <string_view>
#include <unordered_map>

namespace robot::device {
constexpr uint32_t TICK_RATE = 20;

class Device {
public:
  static constexpr uint32_t CONFIGURE_RATE = 500;

private:
  std::string_view typeName;
  std::string_view name;

protected:
  const int8_t port;

public:
  explicit Device(std::string_view typeName, std::string_view name, int8_t port);

  virtual ~Device() = default;

  virtual void reconfigure() const = 0;

  [[nodiscard]] virtual bool isConnected() const = 0;
  [[nodiscard]] int8_t getPort() const;

  [[nodiscard]] std::string_view getTypeName() const;
  [[nodiscard]] std::string_view getName() const;
};

void initialize();

std::unordered_map<Device *, bool> &getDevices();
} // namespace robot::device
#endif // ROBOT_DEVICE_DEVICE_HPP
