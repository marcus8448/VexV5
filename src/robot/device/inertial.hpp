#ifndef ROBOT_DEVICE_INERTIAL_HPP
#define ROBOT_DEVICE_INERTIAL_HPP

#include "device.hpp"

namespace robot::device {
class Inertial final : public Device {
public:
  Inertial(int8_t port, std::string_view name);
  ~Inertial() override = default;

  [[nodiscard]] double getRotation() const;
  [[nodiscard]] double getHeading() const;

  [[nodiscard]] double getYaw() const;
  [[nodiscard]] double getPitch() const;
  [[nodiscard]] double getRoll() const;

  [[nodiscard]] bool isCalibrating() const;
  [[nodiscard]] bool isConnected() const override;
  void reconfigure() const override;
  void tare();
  void calibrate();
};
} // namespace robot::device
#endif // ROBOT_DEVICE_INERTIAL_HPP
