#ifndef ROBOT_DEVICE_DEVICE_HPP
#define ROBOT_DEVICE_DEVICE_HPP

#define ROBOT_TICK_RATE 20

#include <cstdint>
#include <map>

namespace robot::device {
class Device {
private:
  const char *typeName;
  const char *name;

protected:
  const uint8_t port;

public:
  explicit Device(const char *typeName, const char *name, uint8_t port);
  explicit Device(const Device &) = delete;
  ~Device() = default;

  virtual void reconfigure() const = 0;

  [[nodiscard]] virtual bool isConnected() const = 0;
  [[nodiscard]] uint8_t getPort() const;

  [[nodiscard]] const char *getTypeName() const;
  [[nodiscard]] const char *getName() const;

protected:
  [[nodiscard]] static bool checkConnect();
};

void initialize();

std::map<Device *, bool> *get_devices();
} // namespace robot::device
#endif // ROBOT_DEVICE_DEVICE_HPP
