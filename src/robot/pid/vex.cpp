#include "pros/motors.h"
#include "robot/pid/pid.hpp"

namespace robot {
Vex::Vex(uint8_t port) : port(port) {}

Vex::~Vex() = default;

void Vex::reset() {}

void Vex::startTargeting(double targetVelocity) { pros::c::motor_move_velocity(this->port, 0); }

void Vex::update() {}
} // namespace robot
