#include "robot/flywheel.hpp"
#include "logger.hpp"
#include <cmath>

namespace robot {
Flywheel::Flywheel(uint8_t port, uint8_t secondary_port)
    : primaryMotor(device::Motor(port, "Flywheel", pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST, false)),
      secondaryMotor(
          device::Motor(secondary_port, "Flywheel 2", pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST, true)) {
  this->prevSpeeds.reserve(20);
}

Flywheel::~Flywheel() = default;

void Flywheel::engage(int16_t flywheelMV, bool block) {
  if (this->targetMV != flywheelMV) {
    if (this->targetMV < flywheelMV) {
      this->state = State::SPINNING_DOWN;
    } else if (this->targetMV > flywheelMV) {
      this->state = State::SPINNING_UP;
    }

    // if (std::abs(this->targetMV - flywheelMV) > 1000) {
    //   this->first_motor.move_millivolts(12000);
    //   this->second_motor.move_millivolts(12000);
    // } else {
    this->primaryMotor.move_millivolts(flywheelMV);
    this->secondaryMotor.move_millivolts(flywheelMV);
    // }

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

void Flywheel::update(Controller *controller) {
  if (controller->r1_pressed()) {
    this->engage(controller->flywheel_speed());
  } else if (controller->x_pressed()) {
    this->disengage();
  }
  if (this->prevSpeeds.size() == 20) {
    this->prevSpeeds.erase(this->prevSpeeds.begin());
  }
  double velocity = this->primaryMotor.get_velocity();
  this->prevSpeeds.emplace_back(velocity);
  double runMin = 600.0;
  double runMax = 0;
  for (double d : prevSpeeds) {
    runMax = std::max(d, runMax);
    runMin = std::min(d, runMin);
  }

  if (this->state == SPINNING_UP || this->state == SPINNING_DOWN) {
    logger::info("FLY: %f %f %f %f", velocity, runMax, runMin, runMax - runMin);
    if (runMax - runMin < 55.0 && this->prevSpeeds.size() == 20) {
      if (this->state == SPINNING_UP) {
        controller->rumble("-");
        logger::info("SUCCESS: %f", velocity);
      }
      this->state = AT_SPEED;
    }
  }

  if (this->state == State::AT_SPEED) {
    if (runMax - velocity > 80.0) {
      logger::info("regression - spinning up.");
      this->state = State::SPINNING_UP;
    }
  }
}

[[nodiscard]] const device::Motor &Flywheel::get_primary_motor() const { return this->primaryMotor; }
[[nodiscard]] const device::Motor &Flywheel::get_secondary_motor() const { return this->secondaryMotor; }

void Flywheel::reset_speeds() { return this->prevSpeeds.clear(); }

void Flywheel::wait_for_speed(uint16_t millis_timeout) {
  if (this->state == State::IDLE) {
    logger::error("Waiting for flywheel to speed up while it's off!");
    return;
  }
  millis_timeout /= 10;
  int16_t i = 0;

  double runMin = 600.0;
  double runMax = 0;
  for (double d : prevSpeeds) {
    runMax = std::max(d, runMax);
    runMin = std::min(d, runMin);
  }
  while (runMax - runMin > 55.0 || this->prevSpeeds.size() != 20) {
    if (this->prevSpeeds.size() == 20) {
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
    double d = this->primaryMotor.get_velocity();
    logger::info("FLY: %f %f", d, this->primaryMotor.get_raw_motor().get_power());
    this->prevSpeeds.emplace_back(d);
    runMax = std::max(d, runMax);
    runMin = std::min(d, runMin);

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
}
} // namespace robot
