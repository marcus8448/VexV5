#include "robot/pid/velocity_pid.hpp"
#include "logger.hpp"
#include "pros/motors.h"

namespace robot {
VelocityPid::VelocityPid(uint8_t port, double Kp, double Ki, double Kd, double dt)
    : port(port), Kp(Kp), Ki(Ki), Kd(Kd), dt(dt), A0(Kp + Ki * dt), A1(-Kp), A0d(Kd / dt), A1d(-2.0 * Kd / dt),
      A2d(Kd / dt), tau(Kd / (Kp * 5)), alpha(dt / (2 * tau)) {}

VelocityPid::~VelocityPid() {}

void VelocityPid::reset() { this->targetVelocity = 0.0; }

void VelocityPid::target_velocity(double targetVelocity) {
  info("target %f", targetVelocity);
  this->targetVelocity = targetVelocity;
  this->output = pros::c::motor_get_actual_velocity(this->port);
}

void VelocityPid::update() {
  if (this->targetVelocity == 0.0)
    return;
  auto vel = pros::c::motor_get_actual_velocity(this->port);
  error[0] = vel - this->targetVelocity;
  proportional = error[0];
  integral = integral + error[0] * dt;
  derivative = (error[0] - error[1]) / dt;
  error[1] = error[0];
  this->output = Kp * proportional + Ki * integral + Kd * derivative;
  if (this->output > 12000)
    this->output = 12000;
  // if (this->count++ % 100 == 0) {
  info("output: %f (%f / 450)", this->output, vel);
  pros::c::motor_move_voltage(this->port, static_cast<int32_t>(this->output));
  // }
}
} // namespace robot
