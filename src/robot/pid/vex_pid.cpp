#include "robot/pid/vex_pid.hpp"
#include "pros/motors.h"

namespace robot {
VexPid::VexPid(uint8_t port) : port(port) {}

VexPid::~VexPid() = default;

void VexPid::reset() {}

void VexPid::target_velocity(double targetVelocity) { pros::c::motor_move_velocity(this->port, 0); }

void VexPid::update() {}
} // namespace robot
