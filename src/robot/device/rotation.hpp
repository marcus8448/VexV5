#ifndef ROBOT_DEVICE_ROTATION_HPP
#define ROBOT_DEVICE_ROTATION_HPP

#include "device.hpp"

namespace robot::device {
class Rotation final : public Device {
public:
  Rotation(int8_t port, std::string_view name);
  ~Rotation() override = default;

  [[nodiscard]] double getRotation() const;

  [[nodiscard]] bool isConnected() const override;
  void reconfigure() const override;
  void tare();
};
} // namespace robot::device
#endif // ROBOT_DEVICE_ROTATION_HPP
