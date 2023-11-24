#ifndef ROBOT_DEVICE_OPTICAL_HPP
#define ROBOT_DEVICE_OPTICAL_HPP

#include "device.hpp"

namespace robot::device {
class Optical final : public Device {
  uint8_t led_pwm;
  bool gesture = false;

public:
  explicit Optical(int8_t port, std::string_view name, uint8_t led_pwm = 0, bool gesture = false);
  ~Optical() override = default;

  [[nodiscard]] double getHue() const;
  [[nodiscard]] double getSaturation() const;
  [[nodiscard]] double getBrightness() const;

  [[nodiscard]] int32_t getProximity() const;

  void setLedPwm(uint8_t value);
  [[nodiscard]] int32_t getLedPwm() const;

  void enableGesture();
  void disableGesture();

  void reconfigure() const override;

  [[nodiscard]] bool isConnected() const override;
};
} // namespace robot::device

#endif // ROBOT_DEVICE_OPTICAL_HPP
