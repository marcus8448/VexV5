#ifndef ROBOT_DEVICE_GYRO_HPP
#define ROBOT_DEVICE_GYRO_HPP
#include "robot/device/device.hpp"
namespace robot::device {
class Gyro : public Device {
private:
  int32_t handle;

public:
  DEVICE_TYPE_NAME("Gyro")
  Gyro(uint8_t port, const char *name);

  [[nodiscard]] double get_rotation() const;

  [[nodiscard]] bool is_connected() const override;
  void update() override;
  void reconfigure() const override;
};
}
#endif // ROBOT_DEVICE_GYRO_HPP
