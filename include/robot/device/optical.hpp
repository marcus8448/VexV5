#ifndef ROBOT_DEVICE_COLOUR_HPP
#define ROBOT_DEVICE_COLOUR_HPP

#include "device.hpp"

namespace robot::device {
class Optical : public Device {
private:
  uint8_t led_pwm;
  bool gesture = false;

public:
  explicit Optical(uint8_t port, const char *name, uint8_t led_pwm = 0, bool gesture = false);

  [[nodiscard]] double getHue() const;
  [[nodiscard]] double getSaturation() const;
  [[nodiscard]] double getBrightness() const;

  [[nodiscard]] int32_t getProximity() const;

  void setLedPwm(uint8_t value);
  [[nodiscard]] int32_t getLedPwm() const;

  void enableGesture();
  void disableGesture();

  void update() override;
  void reconfigure() const override;

  [[nodiscard]] bool isConnected() const override;
};
} // namespace robot::device

#endif // ROBOT_DEVICE_COLOUR_HPP
