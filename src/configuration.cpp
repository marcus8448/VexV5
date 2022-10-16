#include "configuration.hpp"

namespace config {
static DrivetrainControlScheme controlScheme = DrivetrainControlScheme::TANK_DRIVE;

[[nodiscard]] DrivetrainControlScheme get_drivetrain_control_scheme() { return controlScheme; }

[[nodiscard]] const char *name(DrivetrainControlScheme scheme) {
  switch (scheme) {
  case TANK_DRIVE:
    return "Tank";
  case ARCADE_DRIVE:
    return "Arcade";
  }
}

void set_drivetrain_control_scheme(DrivetrainControlScheme scheme) { controlScheme = scheme; }
} // namespace config
