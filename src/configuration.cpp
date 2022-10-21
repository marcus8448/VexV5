#include "configuration.hpp"

namespace config {
static DrivetrainControlScheme controlScheme = DrivetrainControlScheme::ARCADE_DRIVE;
static AllianceColour allianceColour = AllianceColour::RED;

[[nodiscard]] DrivetrainControlScheme get_drivetrain_control_scheme() { return controlScheme; }

[[nodiscard]] const char *get_scheme_name(DrivetrainControlScheme scheme) {
  switch (scheme) {
  case TANK_DRIVE:
    return "Tank";
  case ARCADE_DRIVE:
    return "Arcade";
  }
  return "";
}

void set_drivetrain_control_scheme(DrivetrainControlScheme scheme) { controlScheme = scheme; }

AllianceColour get_alliance_colour() { return allianceColour; }

const char *get_alliance_colour_name(AllianceColour colour) {
  switch (colour) {
  case RED:
    return "Red";
  case BLUE:
    return "Blue";
  }
  return "";
}

void set_alliance_colour(AllianceColour colour) { allianceColour = colour; }
} // namespace config
