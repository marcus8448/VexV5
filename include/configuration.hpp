#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

/**
 * The runtime configuration of the robot.
 * Can be changed via the V5 brain's screen.
 */
namespace config {
/**
 * The different control schemes for the drivetrain
 */
enum DrivetrainControlScheme {
  /**
   * Left stick y-axis controls the left side.
   * Right stick y-axis controls the right side.
   */
  TANK_DRIVE,

  /**
   * Left stick y-axis determines the overall power.
   * Right stick x-axis determines the balance between the left and right motors.
   */
  ARCADE_DRIVE
};

/**
 * Returns the current drivetrain control scheme.
 * The default control scheme is tank drive.
 * @return The current drivetrain control scheme.
 */
[[nodiscard]] DrivetrainControlScheme get_drivetrain_control_scheme();

/**
 * Sets the drivetrain control scheme to the specified value.
 * @param scheme The drivetrain control scheme to use.
 */
void set_drivetrain_control_scheme(DrivetrainControlScheme scheme);
} // namespace config
#endif // CONFIGURATION_HPP
