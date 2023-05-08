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
  TANK,

  /**
   * Left stick y-axis determines the overall power.
   * Right stick x-axis determines the balance between the left and right motors.
   */
  ARCADE
};

extern DrivetrainControlScheme controlScheme;

[[nodiscard]] const char *driveSchemeName(DrivetrainControlScheme scheme);
} // namespace config
#endif // CONFIGURATION_HPP
