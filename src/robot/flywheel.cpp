#include "robot/flywheel.hpp"
#include "logger.hpp"
#include <cmath>

#define VELCONV(mv) (mv/20.0)

// at  6000: max = 288.800000, min = 262.800000, avg = 273.468293
// at  6250: max = 299.600000, min = 274.600000, avg = 284.751220
// at  6500: max = 316.800000, min = 279.800000, avg = 296.482927
// at  6750: max = 328.800000, min = 298.600000, avg = 313.043902
// at  7000: max = 339.200000, min = 302.000000, avg = 320.824390
// at  7250: max = 359.600000, min = 319.600000, avg = 340.819512
// at  7500: max = 373.400000, min = 336.400000, avg = 354.180488
// at  7750: max = 392.800000, min = 351.200000, avg = 369.839024
// at  8000: max = 400.600000, min = 357.000000, avg = 379.702439
// at  8250: max = 414.400000, min = 374.800000, avg = 394.692683
// at  8500: max = 430.400000, min = 384.000000, avg = 407.014634
// at  8750: max = 442.400000, min = 398.000000, avg = 418.756098
// at  9000: max = 459.800000, min = 415.000000, avg = 438.858537
// at  9250: max = 481.600000, min = 432.400000, avg = 454.678049
// at  9500: max = 491.200000, min = 441.000000, avg = 467.146341
// at  9750: max = 502.000000, min = 447.200000, avg = 479.180488
// at 10000: max = 508.800000, min = 456.800000, avg = 485.917073
// at 10250: max = 527.200000, min = 488.800000, avg = 505.117073
// at 10500: max = 539.400000, min = 479.800000, avg = 511.756098
// at 10750: max = 556.200000, min = 500.800000, avg = 525.014634
// at 11000: max = 567.800000, min = 502.000000, avg = 539.536585
// at 11250: max = 566.400000, min = 515.400000, avg = 547.346341
// at 11500: max = 592.400000, min = 531.000000, avg = 562.112195
// at 11750: max = 585.000000, min = 542.000000, avg = 564.790244
// at 12000: max = 591.600000, min = 552.200000, avg = 571.512195
// velocity * 21 ~= mV

namespace robot {
Flywheel::Flywheel(uint8_t port, uint8_t secondary_port)
    : first_motor(device::Motor(port, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST, false)),
      second_motor(device::Motor(secondary_port, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST, true)) {
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
    //   this->first_motor.move_millivolts(flywheelMV);
    //   this->second_motor.move_millivolts(flywheelMV);
    // } else {
      this->first_motor.move_millivolts(flywheelMV);
      this->second_motor.move_millivolts(flywheelMV);
    // }

    this->targetMV = flywheelMV;
    this->prevSpeeds.clear();

    if (block) {
      this->wait_for_speed();
    }
  }
  double targetVelocity = VELCONV(flywheelMV);
}

void Flywheel::disengage() {
  this->state = State::IDLE;
  this->targetMV = 0;
  this->first_motor.stop();
  this->second_motor.stop();
  this->prevSpeeds.clear();
}

void Flywheel::update(Controller *controller) {
  if (this->state != IDLE) logger::info("%i, %f, %f", this->state, this->first_motor.get_velocity(), VELCONV(this->targetMV));
  if (controller->r1_pressed()) {
    this->engage(controller->flywheel_speed());
  } else if (controller->x_pressed()) {
    this->disengage();
  }
  if (this->prevSpeeds.size() == 20) {
    this->prevSpeeds.erase(this->prevSpeeds.begin());
  }
  double velocity = this->first_motor.get_velocity();
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

[[nodiscard]] const device::Motor &Flywheel::get_first_motor() const { return this->first_motor; }
[[nodiscard]] const device::Motor &Flywheel::get_second_motor() const { return this->second_motor; }

double Flywheel::get_first_motor_velocity() { return this->first_motor.get_velocity(); }
double Flywheel::get_second_motor_velocity() { return this->second_motor.get_velocity(); }
void Flywheel::reset_speeds() { return this->prevSpeeds.clear(); }

bool Flywheel::is_up_to_speed() {
  return (std::abs(std::abs(this->first_motor.get_velocity()) - std::abs(VELCONV(this->targetMV))) <
             10.0 &&
         std::abs(std::abs(this->second_motor.get_velocity()) - std::abs(VELCONV(this->targetMV))) <
             10.0);
}

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
    double d = this->first_motor.get_velocity();
    logger::info("FLY: %f %f", d, this->first_motor.get_raw_motor().get_power());
    this->prevSpeeds.emplace_back(d);
    runMax = std::max(d, runMax);
    runMin = std::min(d, runMin);

    // if (runMax - runMin > 100.0) {
    //   this->prevSpeeds.clear();
    // }

    if (i++ == millis_timeout) break; 
    pros::delay(10);
  }
  this->first_motor.move_millivolts(this->targetMV);
  this->second_motor.move_millivolts(this->targetMV);
  this->state = State::AT_SPEED;
}
} // namespace robot
