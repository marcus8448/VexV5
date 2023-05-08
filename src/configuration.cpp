#include "configuration.hpp"

namespace config {
#ifndef ARCADE_DRIVE
DrivetrainControlScheme controlScheme = TANK;
#else
DrivetrainControlScheme controlScheme = ARCADE;
#endif

[[nodiscard]] const char *driveSchemeName(DrivetrainControlScheme scheme) {
  switch (scheme) {
  case TANK:
    return "Tank";
  case ARCADE:
    return "Arcade";
  }
  return "";
}
} // namespace config
