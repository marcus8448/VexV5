#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

namespace config {
enum DrivetrainControlScheme { TANK_DRIVE, ARCADE_DRIVE };

DrivetrainControlScheme get_drivetrain_control_scheme();
void set_drivetrain_control_scheme(DrivetrainControlScheme style);
} // namespace config
#endif // CONFIGURATION_HPP
