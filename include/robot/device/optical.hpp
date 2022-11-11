#ifndef ROBOT_DEVICE_COLOUR_HPP
#define ROBOT_DEVICE_COLOUR_HPP

#include "pros/optical.hpp"
#include "robot/device/device.hpp"

namespace robot::device {
class ColourSensor : public Device {
private:
  pros::Optical optical;
  const uint8_t port;

public:
  explicit ColourSensor(uint8_t port);

  double get_hue();
  double get_saturation();
  double get_brightness();

  int32_t get_proximity();

  int32_t set_led_pwm(uint8_t value);
  int32_t get_led_pwm();

  bool disable_gesture();

  bool is_connected() override;
  uint8_t get_port() const override;
};
} // namespace robot::device

#endif // ROBOT_DEVICE_COLOUR_HPP
