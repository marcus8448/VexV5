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
  explicit Optical(uint8_t port, uint8_t led_pwm = 0, bool gesture = false);

  double get_hue() const;
  double get_saturation() const;
  double get_brightness() const;

  int32_t get_proximity() const;

  void set_led_pwm(uint8_t value);
  int32_t get_led_pwm() const;

  void enable_gesture();
  void disable_gesture();

  void reconfigure() const override;

  bool is_connected() const override;
};
} // namespace robot::device

#endif // ROBOT_DEVICE_COLOUR_HPP
