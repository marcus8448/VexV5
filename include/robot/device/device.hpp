#ifndef ROBOT_DEVICE_DEVICE_HPP
#define ROBOT_DEVICE_DEVICE_HPP

#include <cstdint>
#include <map>

namespace robot::device {
constexpr uint32_t TICK_RATE = 20;

class Device {
private:
  const char *typeName;
  const char *name;

protected:
  const int8_t port;

public:
  explicit Device(const char *typeName, const char *name, int8_t port);
  explicit Device(const Device &) = delete;

  virtual ~Device() = default;

  virtual void reconfigure() const = 0;

  [[nodiscard]] virtual bool isConnected() const = 0;
  [[nodiscard]] int8_t getPort() const;

  [[nodiscard]] const char *getTypeName() const;
  [[nodiscard]] const char *getName() const;

protected:
  [[nodiscard]] static bool checkConnect();
};

void initialize();

std::map<Device *, bool> *get_devices();
} // namespace robot::device
#endif // ROBOT_DEVICE_DEVICE_HPP
