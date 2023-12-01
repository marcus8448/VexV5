#ifndef ROBOT_DEVICE_PNEUMATICS_HPP
#define ROBOT_DEVICE_PNEUMATICS_HPP

#include "robot/device/device.hpp"
#include <cstdint>

namespace robot::device {
class PneumaticPiston final : public Device {
  bool extended;

public:
  explicit PneumaticPiston(int8_t port, std::string_view name, bool defaultState = false);
  ~PneumaticPiston() override = default;

  void extend();
  void contract();
  void toggle();

  [[nodiscard]] bool isExtended() const;

  void reconfigure() const override;
  [[nodiscard]] bool isConnected() const override;
};
} // namespace robot::device
#endif // ROBOT_DEVICE_PNEUMATICS_HPP
