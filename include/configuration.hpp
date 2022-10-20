#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

/**
 * The runtime configuration of the robot.
 * Can be changed via the V5 brain's screen.
 */
namespace config {
enum AllianceColour {
  RED,
  BLUE
};

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

[[nodiscard]] const char *get_scheme_name(DrivetrainControlScheme scheme);
/**
 * Sets the drivetrain control scheme to the specified value.
 * @param scheme The drivetrain control scheme to use.
 */
void set_drivetrain_control_scheme(DrivetrainControlScheme scheme);

[[nodiscard]] AllianceColour get_alliance_colour();

[[nodiscard]] const char *get_alliance_colour_name(AllianceColour colour);

/**
 * Sets the alliance colour to the specified value.
 * @param colour The drivetrain control scheme to use.
 */
void set_alliance_colour(AllianceColour colour);
} // namespace config
#endif // CONFIGURATION_HPP
