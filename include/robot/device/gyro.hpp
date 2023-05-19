#ifndef ROBOT_DEVICE_GYRO_HPP
#define ROBOT_DEVICE_GYRO_HPP

#include "device.hpp"

namespace robot::device {
class Gyro : public Device {
private:
  int32_t handle;

public:
  Gyro(uint8_t port, const char *name);

  [[nodiscard]] double getRotation() const;

  [[nodiscard]] bool isConnected() const override;
  void update() override;
  void reconfigure() const override;
};
} // namespace robot::device
#endif // ROBOT_DEVICE_GYRO_HPP
