#include "robot/intake.hpp"
#include "logger.hpp"
#include "pros/optical.hpp"
#include "robot/device/motor.hpp"
#include "robot/device/optical.hpp"

namespace robot {

Intake::Intake(uint8_t motorPort, uint8_t colourPort)
    : motor(device::Motor(motorPort, pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_BRAKE, false)),
      colour(device::ColourSensor(colourPort)) {}

Intake::~Intake() = default;

void Intake::engage(int16_t target_millivolts) {
  this->motor.set_reversed(true);
  this->motor.move_millivolts(target_millivolts);
  this->engaged = true;
}

void Intake::reverse(int16_t target_millivolts) {
  this->motor.set_reversed(false);
  this->motor.move_millivolts(target_millivolts);
  this->engaged = true;
}

void Intake::disengage() {
  this->engaged = false;
  this->motor.stop();
}

void Intake::roll_to_team_colour(config::AllianceColour teamColour, uint32_t timeout) {
  if (!this->colour.is_connected()) {
    logger::warn("Colour sensor is not connected. Spinning the roller anyways.");
    this->reverse(4800);
    pros::delay(750);
    this->disengage();
  } else {
    // if (!looking_at_roller()) {
    //   logger::warn("Spinning roller while not in view?");
    // }
    double hue = this->colour.get_hue();
    uint32_t i = 0;
    timeout /= 50;
    switch (teamColour) {
    case config::AllianceColour::RED: {
      while (hue < 335.0 || hue > 15.0) {
        this->motor.move_percentage(50.0);
        pros::delay(50);
        if (i++ == timeout)
          break;
      }
    }
    case config::AllianceColour::BLUE: {
      while (hue < 210.0 || hue > 240.0) {
        this->motor.move_percentage(50.0);
        pros::delay(50);
        if (i++ == timeout)
          break;
      }
    }
    }
    this->motor.stop();
  }
}

[[nodiscard]] bool Intake::isEngaged() const { return this->engaged; }

[[nodiscard]] const device::Motor &Intake::get_motor() const { return this->motor; }

void Intake::update(Controller *controller) {
  if (controller->l1_pressed() && controller->l2_pressed()) {
    this->reverse();
  } else if (controller->l1_pressed()) {
    this->engage();
  } else if (controller->l2_pressed()) {
    this->disengage();
  }
}
} // namespace robot
