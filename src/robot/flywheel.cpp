#include "robot/flywheel.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"
#include <cmath>

#define FLYWHEEL_VARIANCE 0.876
#define FLYWHEEL_SAMPLES 50

namespace robot {
Flywheel::Flywheel(uint8_t port, uint8_t secondary_port)
    : primaryMotor(device::Motor(port, "Flywheel", pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST, false)),
      secondaryMotor(
          device::Motor(secondary_port, "Flywheel 2", pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST, true)) {
  this->prevSpeeds.reserve(FLYWHEEL_SAMPLES);
}

Flywheel::~Flywheel() = default;

void Flywheel::engage(int16_t flywheelMV, bool block) {
  if (this->targetMV != flywheelMV) {
    if (this->targetMV > flywheelMV) {
      this->state = State::SPINNING_DOWN;
    } else if (this->targetMV < flywheelMV) {
      this->state = State::SPINNING_UP;
    }

    this->primaryMotor.move_millivolts(flywheelMV);
    this->secondaryMotor.move_millivolts(flywheelMV);

    this->targetMV = flywheelMV;
    this->prevSpeeds.clear();

    if (block) {
      this->wait_for_speed();
    }
  }
}

void Flywheel::disengage() {
  this->state = State::IDLE;
  this->targetMV = 0;
  this->primaryMotor.stop();
  this->secondaryMotor.stop();
  this->prevSpeeds.clear();
}

void Flywheel::update(Controller *controller) { // todo: run at max once target is known.
  if (controller->r1_pressed()) {
    this->engage(controller->flywheel_speed());
  } else if (controller->x_pressed()) {
    this->disengage();
  }
  if (this->prevSpeeds.size() == FLYWHEEL_SAMPLES) {
    this->prevSpeeds.erase(this->prevSpeeds.begin());
  }
  double velocity = this->primaryMotor.get_velocity();
  this->prevSpeeds.emplace_back(velocity);
  double runMin = 600.0;
  double runMax = 0;
  double runTotal = 0;
  for (double d : prevSpeeds) {
    runMax = std::max(d, runMax);
    runMin = std::min(d, runMin);
    runTotal += d;
  }

  if (this->state == SPINNING_UP || this->state == SPINNING_DOWN) {
    debug("Flywheel - vel: %f, max: %f, min: %f, diff: %f, pcnt: %f, avg: %f", velocity, runMax, runMin,
          runMax - runMin, runMin / runMax, runTotal / static_cast<double>(FLYWHEEL_SAMPLES));
    if ((runMin / runMax > FLYWHEEL_VARIANCE && runMin / runMax < 1.0) && this->prevSpeeds.size() == FLYWHEEL_SAMPLES) {
      if (this->state == SPINNING_UP) {
        controller->rumble("-");
        info("Flywheel up to speed - see ^", velocity);
      }
      this->state = AT_SPEED;
    }
  }

  if (this->state == State::AT_SPEED) {
    if (runMax - velocity > 80.0) {
      info("Speed regression - spinning up.");
      this->state = State::SPINNING_UP;
    }
  }
}

[[nodiscard]] const device::Motor &Flywheel::get_primary_motor() const { return this->primaryMotor; }
[[nodiscard]] const device::Motor &Flywheel::get_secondary_motor() const { return this->secondaryMotor; }

void Flywheel::reset_speeds() { return this->prevSpeeds.clear(); }

void Flywheel::wait_for_speed(uint16_t millis_timeout) {
  if (this->state == State::IDLE) {
    error("Waiting for flywheel to speed up while it's off!");
    return;
  }
  uint32_t start = pros::millis();
  millis_timeout /= 10;
  int16_t i = 0;

  double runMin = 600.0;
  double runMax = 0.0;
  for (double d : prevSpeeds) {
    runMax = std::max(d, runMax);
    runMin = std::min(d, runMin);
  }
  while ((runMin / runMax > FLYWHEEL_VARIANCE && runMin / runMax < 1.0) ||
         this->prevSpeeds.size() != FLYWHEEL_SAMPLES) {
    if (this->prevSpeeds.size() == FLYWHEEL_SAMPLES) {
      double rem = *this->prevSpeeds.erase(this->prevSpeeds.begin());
      if (runMax == rem || runMin == rem) {
        runMax = 0;
        runMin = 600;
        for (double d : prevSpeeds) {
          runMax = std::max(d, runMax);
          runMin = std::min(d, runMin);
        }
      }
    }
    double velocity = this->primaryMotor.get_velocity();
    debug("Flywheel (a) - vel: %f, max: %f, min: %f, diff: %f", velocity, runMax, runMin, runMax / runMin);
    this->prevSpeeds.emplace_back(velocity);
    runMax = std::max(velocity, runMax);
    runMin = std::min(velocity, runMin);

    // if (runMax - runMin > 100.0) {
    //   this->prevSpeeds.clear();
    // }

    if (i++ == millis_timeout)
      break;
    pros::delay(10);
  }
  this->primaryMotor.move_millivolts(this->targetMV);
  this->secondaryMotor.move_millivolts(this->targetMV);
  this->state = State::AT_SPEED;
  info("Flywheel (a) up to speed - elapsed: %i", pros::millis() - start);
}
} // namespace robot
