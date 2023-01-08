#include "robot/intake.hpp"
#include "logger.hpp"
#include "pros/rtos.hpp"
#include "robot/device/motor.hpp"
#include "robot/device/optical.hpp"

#define ROLLER_VARIANCE 15.0

namespace robot {

Intake::Intake(uint8_t motorPort)
    : motor(device::Motor(motorPort, "Intake/Roller", pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_HOLD, false)) {}

Intake::~Intake() = default;

void Intake::engage(int16_t mV) {
  if (!this->reversed || this->speed != mV) {
    debug("Intake engaged at %imV", mV);
    this->motor.set_reversed(true);
    this->motor.move_millivolts(mV);
    this->reversed = true;
    this->speed = mV;
  }
}

void Intake::reverse(int16_t mV) {
  if (this->reversed || this->speed != mV) {
    debug("Intake engaged at %imV", mV);
    this->motor.set_reversed(false);
    this->motor.move_millivolts(mV);
    this->reversed = false;
    this->speed = mV;
  }
}

void Intake::disengage() {
  if (this->speed != 0) {
    debug("Intake disengaged");
    this->speed = 0;
    this->motor.stop();
  }
}

[[nodiscard]] bool Intake::is_engaged() const { return this->speed != 0; }

[[nodiscard]] const device::Motor &Intake::get_motor() const { return this->motor; }

void Intake::update(Controller *controller) {
  if (controller->l1_pressed() && controller->l2_pressed()) {
    this->reverse();
  } else if (controller->l1_pressed() == 3) {
    this->engage();
  } else if (controller->l2_pressed() == 3) {
    this->disengage();
  }
}

double Intake::bring_roller_to_speed(int16_t mV) {
  std::vector<double> speeds = std::vector<double>(10);
  this->reverse(mV);
  pros::delay(250);

  double runMin = 200.0;
  double runMax = 0;
  while (runMax - runMin > ROLLER_VARIANCE || speeds.size() != 20) {
    if (speeds.size() == 20) {
      double rem = *speeds.erase(speeds.begin());
      if (runMax == rem || runMin == rem) {
        runMax = 0;
        runMin = 600;
        for (double d : speeds) {
          runMax = std::max(d, runMax);
          runMin = std::min(d, runMin);
        }
      }
    }
    double d = this->motor.get_velocity();
    speeds.emplace_back(d);
    runMax = std::max(d, runMax);
    runMin = std::min(d, runMin);

    pros::delay(10);
  }
  info("Roller up to speed (min: %f, max: %f, diff: %f)", runMin, runMax, runMax - runMin);
  return runMin;
}
} // namespace robot
