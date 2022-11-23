#ifndef ROBOT_DEVICE_PNEUMATICS_HPP
#define ROBOT_DEVICE_PNEUMATICS_HPP

typedef __SIZE_TYPE__ size_t; // adi.hpp requires it

#include "pros/adi.hpp"
#include "robot/device/device.hpp"
#include <cstdint>

namespace robot::device {
class PneumaticPiston : public Device {
private:
  const char port;
  const pros::ADIDigitalOut piston;
  bool extended;

public:
  explicit PneumaticPiston(char port);
  explicit PneumaticPiston(char port, bool defaultState);

  void extend();
  void contract();
  void toggle();
  
  [[nodiscard]] bool is_extended();

  [[nodiscard]] virtual uint8_t get_port() const override;
  [[nodiscard]] virtual bool is_connected() override;

  [[nodiscard]] pros::ADIDigitalOut get_raw_piston() const;
};
} // namespace robot::device
#endif // ROBOT_DEVICE_PNEUMATICS_HPP
