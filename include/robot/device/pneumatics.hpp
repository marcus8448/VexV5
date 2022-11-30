#ifndef ROBOT_DEVICE_PNEUMATICS_HPP
#define ROBOT_DEVICE_PNEUMATICS_HPP

typedef __SIZE_TYPE__ size_t; // adi.hpp requires it

#include "pros/adi.hpp"
#include "robot/device/device.hpp"
#include <cstdint>

namespace robot::device {
class PneumaticPiston : public Device {
private:
  const pros::ADIDigitalOut piston;
  bool extended;

public:
  DEVICE_NAME("Pneumatic Piston")
  explicit PneumaticPiston(uint8_t port);
  explicit PneumaticPiston(uint8_t port, bool defaultState);

  void extend();
  void contract();
  void toggle();

  [[nodiscard]] bool is_extended() const;

  void reconfigure() const override;
  [[nodiscard]] bool is_connected() const override;

  [[nodiscard]] pros::ADIDigitalOut get_raw_piston() const;
};
} // namespace robot::device
#endif // ROBOT_DEVICE_PNEUMATICS_HPP
