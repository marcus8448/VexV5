#ifndef ROBOT_DEVICE_DEVICE_HPP
#define ROBOT_DEVICE_DEVICE_HPP

#define ROBOT_TICK_RATE 20

#define DEVICE_TYPE_NAME(type_name)                                                                                    \
  [[nodiscard]] const char *get_type_name() const override { return type_name; };

#include "robot/updatable.hpp"
#include <cstdint>

namespace robot::device {
class Device : public Updatable {
public:
  explicit Device(uint8_t port, const char *name);
  explicit Device(const Device &) = delete;
  ~Device() override = default;

  virtual void reconfigure() const = 0;

  [[nodiscard]] virtual bool is_connected() const = 0;
  [[nodiscard]] uint8_t get_port() const;

  [[nodiscard]] virtual const char *get_type_name() const = 0;
  [[nodiscard]] const char *get_name() const;

protected:
  const uint8_t port;
  const char *name;

  [[nodiscard]] static bool checkConnect();
};

void initialize();
} // namespace robot::device
#endif // ROBOT_DEVICE_DEVICE_HPP
