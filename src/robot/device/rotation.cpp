#include "rotation.hpp"
#include "debug/error.hpp"
#include "debug/logger.hpp"
#include "pros/rotation.h"

namespace robot::device {
Rotation::Rotation(int8_t port, std::string_view name, bool reversed) : Device("Rotation", name, port) {
  pros::c::rotation_set_reversed(port, reversed);
}

double Rotation::getRotation() const {
  const double rotation = pros::c::rotation_get_angle(this->port);
  if (error::check(rotation)) {
    error::print(this->getName());
    return 0.0;
  }
  return rotation;
}

bool Rotation::isConnected() const { return error::check(pros::c::rotation_get_angle(this->port)); }

void Rotation::reconfigure() const {}

void Rotation::tare() { pros::c::rotation_set_position(this->port, 0); }
} // namespace robot::device
