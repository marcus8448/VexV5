#include "robot/flywheel.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"
#include "robot/pid/tbh.hpp"
#include <cmath>

#define FLYWHEEL_VARIANCE 0.918
#define FLYWHEEL_SAMPLES 50

// shooting across the field: 10100
// shooting at point blank: 8700

namespace robot {
Flywheel::Flywheel(uint8_t port, uint8_t secondary_port)
    : primaryMotor(device::Motor(port, "Flywheel", pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST, false)),
      secondaryMotor(
          device::Motor(secondary_port, "Flywheel 2", pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST, true)) {
  this->prevSpeeds.reserve(FLYWHEEL_SAMPLES);
  this->primaryMotor.controller = new TBHControl(port, secondary_port, 4.8);
  this->secondaryMotor.controller = nullptr;
}

Flywheel::~Flywheel() = default;

//void Flywheel::engage(int16_t flywheelMV, bool block) {
//  if (this->targetMV != flywheelMV) {
//    if (this->targetMV > flywheelMV) {
//      this->state = State::SPINNING_DOWN;
//    } else if (this->targetMV < flywheelMV) {
//      this->state = State::SPINNING_UP;
//    }
//
//    this->primaryMotor.move_millivolts(flywheelMV);
//    this->secondaryMotor.move_millivolts(flywheelMV);
//
//    // this->primaryMotor.move_velocity(450);
//    // this->secondaryMotor.move_velocity(450);
//
//    this->targetMV = flywheelMV;
//    this->prevSpeeds.clear();
//
//    if (block) {
//      this->wait_for_speed();
//    }
//  }
//}

void Flywheel::engage_velocity(double velocity, bool block) {
  this->primaryMotor.move_velocity(velocity);
  this->secondaryMotor.move_velocity(velocity);

  this->prevSpeeds.clear();

  if (block) {
    this->wait_for_speed();
  }
}

void Flywheel::disengage() {
  this->state = State::IDLE;
  this->targetVelocity = 0;
  this->primaryMotor.brake();
  this->secondaryMotor.brake();
  this->prevSpeeds.clear();
}

void Flywheel::update(Controller *controller) { // todo: run at max once target is known.
  this->primaryMotor.update();
  if (controller->r1_pressed()) {
    this->engage_velocity(controller->flywheel_speed(), false);
  } else if (controller->x_pressed()) {
    this->disengage();
  }
  double x = 0.0;
  if (this->prevSpeeds.size() == FLYWHEEL_SAMPLES) {
    x = *this->prevSpeeds.begin();
    this->prevSpeeds.erase(this->prevSpeeds.begin());
  }
  double velocity = (this->primaryMotor.get_velocity() + this->secondaryMotor.get_velocity()) / 2.0;
  this->prevSpeeds.emplace_back(velocity);
  double runMin = 600.0;
  double runMax = 0;
  double runTotal = 0;
  double prevTotal = 0;
  for (double d : prevSpeeds) {
    runMax = std::max(d, runMax);
    runMin = std::min(d, runMin);
    runTotal += d;
  }
  prevTotal = runTotal - velocity + x;

//  if (velocity != 0.0) {
//     debug("Flywheel %i - vel: %f, max: %f, min: %f, diff: %f, pcnt: %f, avg: %f", pros::millis(), velocity, runMax,
//           runMin, runMax - runMin, runMin / runMax, runTotal / static_cast<double>(FLYWHEEL_SAMPLES));
//  }

  if (this->state == SPINNING_UP || this->state == SPINNING_DOWN) {

    if (((runMin / runMax > FLYWHEEL_VARIANCE && runMin / runMax < 1.0) || prevTotal > runTotal) &&
        this->prevSpeeds.size() == FLYWHEEL_SAMPLES) {
      if (this->state == SPINNING_UP) {
         controller->rumble(".");
        info("Flywheel up to speed - %f", velocity);
      }
      this->state = AT_SPEED;
    }
  }

  if (this->state == State::AT_SPEED) {
    if (runMax - velocity > 80.0) {
      // info("Speed regression - spinning up.");
      // this->state = State::SPINNING_UP;
    }
  }

}

[[nodiscard]] const device::Motor &Flywheel::get_primary_motor() const { return this->primaryMotor; }
[[nodiscard]] const device::Motor &Flywheel::get_secondary_motor() const { return this->secondaryMotor; }

void Flywheel::reset_speeds() { return this->prevSpeeds.clear(); }

double Flywheel::wait_for_speed(double targetVelocity, uint16_t millis_timeout) {
  if (this->state == State::IDLE) {
    error("Waiting for flywheel to speed up while it's off!");
    return 0.0;
  }
  uint32_t start = pros::millis();
  millis_timeout /= 10;
  if (targetVelocity < 0) {
    int16_t i = 0;

    double runMin = 600.0;
    double runMax = 0.0;
    double total = 0.0;
    this->prevSpeeds.clear();
    //  for (double d : prevSpeeds) {
    //    runMax = std::max(d, runMax);
    //    runMin = std::min(d, runMin);
    //    total += d;
    //  }
    do {
      if (this->prevSpeeds.size() == FLYWHEEL_SAMPLES) {
        double rem = *this->prevSpeeds.erase(this->prevSpeeds.begin());
        total -= rem;
        if (runMax == rem || runMin == rem) {
          runMax = 0;
          runMin = 600;
          for (double d : prevSpeeds) {
            runMax = std::max(d, runMax);
            runMin = std::min(d, runMin);
          }
        }
      }
      double velocity = this->get_velocity();
      debug("Flywheel (a) - vel: %f, max: %f, min: %f, diff: %f", velocity, runMax, runMin, runMin / runMax);
      this->prevSpeeds.emplace_back(velocity);
      total += velocity;
      runMax = std::max(velocity, runMax);
      runMin = std::min(velocity, runMin);

      // if (runMax - runMin > 100.0) {
      //   this->prevSpeeds.clear();
      // }

      if (i++ == millis_timeout)
        break;
      pros::delay(8);
    } while (runMin / runMax < FLYWHEEL_VARIANCE || this->prevSpeeds.size() != FLYWHEEL_SAMPLES);

    this->state = State::AT_SPEED;
    info("Flywheel (a) up to speed - elapsed: %i", pros::millis() - start);
    return total / static_cast<double>(FLYWHEEL_SAMPLES);
  } else {
    int16_t i = 0;
    while (true) {
      double vel = this->get_velocity();
      debug("Flywheel at %f / %f", vel, targetVelocity);
      if (vel >= targetVelocity) {
        i++;
        break;
      } else {
        i = 0;
      }
      pros::delay(8);
    }

    this->state = State::AT_SPEED;
    info("Flywheel (a) up to speed - elapsed: %i", pros::millis() - start);
    return this->get_velocity();
  }
}

double Flywheel::get_velocity() {
  return (this->primaryMotor.get_velocity() + this->secondaryMotor.get_velocity()) / 2.0;
}
} // namespace robot
