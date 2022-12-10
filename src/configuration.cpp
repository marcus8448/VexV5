#include "configuration.hpp"

namespace config {
static Configuration *instance = new Configuration(AllianceColour::BLUE, DrivetrainControlScheme::TANK);

Configuration *get_instance() { return instance; }

Configuration::Configuration(AllianceColour color, DrivetrainControlScheme scheme)
    : allianceColour(color), drivetrainControlScheme(scheme) {}

[[nodiscard]] DrivetrainControlScheme Configuration::get_control_scheme() { return this->drivetrainControlScheme; }

void Configuration::set_drivetrain_control_scheme(DrivetrainControlScheme scheme) {
  this->drivetrainControlScheme = scheme;
}

AllianceColour Configuration::get_alliance_colour() { return this->allianceColour; }

void Configuration::set_alliance_colour(AllianceColour colour) { this->allianceColour = colour; }

[[nodiscard]] const char *get_drive_scheme_name(DrivetrainControlScheme scheme) {
  switch (scheme) {
  case TANK:
    return "Tank";
  case ARCADE:
    return "Arcade";
  }
  return "";
}

const char *get_alliance_colour_name(AllianceColour colour) {
  switch (colour) {
  case RED:
    return "Red";
  case BLUE:
    return "Blue";
  }
  return "";
}
} // namespace config
