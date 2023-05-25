#ifndef ROBOT_DEVICE_PNEUMATICS_HPP
#define ROBOT_DEVICE_PNEUMATICS_HPP

#include "robot/device/device.hpp"
#include <cstdint>

namespace robot::device {
class PneumaticPiston : public Device {
private:
  bool extended;

public:
  explicit PneumaticPiston(uint8_t port, const char *name, bool defaultState = false);

  void extend();
  void contract();
  void toggle();

  [[nodiscard]] bool isExtended() const;

  void reconfigure() const override;
  [[nodiscard]] bool isConnected() const override;
};
} // namespace robot::device
#endif // ROBOT_DEVICE_PNEUMATICS_HPP
