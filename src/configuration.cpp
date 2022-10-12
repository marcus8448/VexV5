#include "configuration.hpp"

namespace config {
static DrivetrainControlScheme scheme = DrivetrainControlScheme::TANK_DRIVE;

DrivetrainControlScheme get_drivetrain_control_scheme() { return scheme; }

void set_drivetrain_control_scheme(DrivetrainControlScheme scheme) { config::scheme = scheme; }
} // namespace config