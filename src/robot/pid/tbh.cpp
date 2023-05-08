#include "robot/pid/tbh.hpp"
#include "debug/logger.hpp"
#include "pros/motors.h"
#include <cmath>

namespace robot {
TbhControl::TbhControl(uint8_t port, uint8_t port2, double gain) : port(port), port2(port2), gain(gain) {}

TbhControl::~TbhControl() = default;

void TbhControl::reset() { this->target = 0.0; }

void TbhControl::startTargeting(double velocity) {
  info("target %f", velocity);
  this->target = velocity;
  this->output = velocity * 16.0; // guess
  this->tbh = velocity * 16.0;
}

void TbhControl::update() {
  if (this->target == 0.0)
    return;
  auto vel = (pros::c::motor_get_actual_velocity(this->port) + pros::c::motor_get_actual_velocity(this->port2)) / 2.0;
  double error = this->target - vel;
  this->output += this->gain * error;
  if (std::signbit(error) != std::signbit(this->prevError)) {
    this->output = 0.5 * (this->output + this->tbh);
    this->tbh = this->output;
    this->prevError = error;
  }
  if (this->output > 12000)
    this->output = 12000;
  info("out %f %f", this->output, this->tbh);
  pros::c::motor_move_voltage(this->port, static_cast<int32_t>(this->output));
  pros::c::motor_move_voltage(this->port2, static_cast<int32_t>(this->output));
}
} // namespace robot
