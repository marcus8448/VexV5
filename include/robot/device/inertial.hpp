#ifndef ROBOT_DEVICE_INERTIAL_HPP
#define ROBOT_DEVICE_INERTIAL_HPP
#include "robot/device/device.hpp"
namespace robot::device {
class Inertial : public Device {
public:
  DEVICE_TYPE_NAME("Inertial")
  Inertial(uint8_t port, const char *name);

  [[nodiscard]] double get_heading() const;

  [[nodiscard]] double get_yaw() const;
  [[nodiscard]] double get_pitch() const;
  [[nodiscard]] double get_roll() const;

  [[nodiscard]] bool is_connected() const override;
  void update() override;
  void reconfigure() const override;
  void tare();
  void calibrate();
};
}
#endif // ROBOT_DEVICE_INERTIAL_HPP
