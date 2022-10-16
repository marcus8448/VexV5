#include "configuration.hpp"

namespace config {
static DrivetrainControlScheme controlScheme = DrivetrainControlScheme::TANK_DRIVE;

[[nodiscard]] DrivetrainControlScheme get_drivetrain_control_scheme() { return controlScheme; }

void set_drivetrain_control_scheme(DrivetrainControlScheme scheme) { controlScheme = scheme; }
} // namespace config