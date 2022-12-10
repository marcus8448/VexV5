#ifndef ROBOT_DEVICE_COLOUR_HPP
#define ROBOT_DEVICE_COLOUR_HPP

#include "pros/optical.hpp"
#include "robot/device/device.hpp"

namespace robot::device {
class Optical : public Device {
private:
  // pros::Optical optical; // for some reason nothing is const
  uint8_t led_pwm;
  bool gesture = false;

public:
  DEVICE_TYPE_NAME("Optical")
  explicit Optical(uint8_t port, const char *name, uint8_t led_pwm = 0, bool gesture = false);

  [[nodiscard]] double get_hue() const;
  [[nodiscard]] double get_saturation() const;
  [[nodiscard]] double get_brightness() const;

  [[nodiscard]] int32_t get_proximity() const;

  void set_led_pwm(uint8_t value);
  [[nodiscard]] int32_t get_led_pwm() const;

  void enable_gesture();
  void disable_gesture();

  void reconfigure() const override;

  [[nodiscard]] bool is_connected() const override;
};
} // namespace robot::device

#endif // ROBOT_DEVICE_COLOUR_HPP
