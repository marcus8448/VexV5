#include "robot/intake.hpp"
#include "logger.hpp"
#include "pros/optical.hpp"
#include "robot/device/motor.hpp"
#include "robot/device/optical.hpp"

namespace robot {

Intake::Intake(uint8_t motorPort, uint8_t upperColourPort, uint8_t lowerColourPort)
    : motor(device::Motor(motorPort, "Intake/Roller", pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_HOLD, false)),
      upperOptical(device::Optical(upperColourPort, "Upper Optical")),
      lowerOptical(device::Optical(lowerColourPort, "Lower Optical")) {}

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
  this->lowerOptical.set_led_pwm(100);
  // this->upperColour.set_led_pwm(100);
  // 20-80 bot
  if (!this->upperOptical.is_connected()) {
    logger::warn("Colour sensor is not connected. Spinning the roller anyways.");
    this->reverse(4800);
    pros::delay(750);
    this->disengage();
  } else {
    // if (!looking_at_roller()) {
    //   logger::warn("Spinning roller while not in view?");
    // }
    double upperHue = this->upperOptical.get_hue();
    double lowerHue = this->lowerOptical.get_hue();
    uint32_t i = 0;
    int16_t ticksProper = 0;
    timeout /= 10;
    this->motor.set_reversed(false);

    switch (teamColour) {
    case config::AllianceColour::RED: {
      while (i++ != timeout) {
        if (i % 250 == 249) {
          this->motor.set_reversed(!this->motor.is_reversed());
        }
        if (is_red(upperHue) && !is_lower_red(lowerHue)) {
          if (ticksProper++ == 15) {
            break;
          }
        } else {
          ticksProper = 0;
        }
        this->motor.move_percentage(50.0);
        pros::delay(10);
        upperHue = this->upperOptical.get_hue();
        lowerHue = this->lowerOptical.get_hue();
      }
      break;
    }
    case config::AllianceColour::BLUE: {
      while (i++ != timeout) {
        if (i % 250 == 249) {
          this->motor.set_reversed(!this->motor.is_reversed());
        }
        if (is_blue(upperHue) && !is_lower_blue(lowerHue)) {
          if (ticksProper++ == 15) {
            break;
          }
        } else {
          ticksProper = 0;
        }
        this->motor.move_percentage(50.0);
        pros::delay(10);
        upperHue = this->upperOptical.get_hue();
        lowerHue = this->lowerOptical.get_hue();
      }
      break;
    }
    }
    logger::info("Successfully rolled to %s (upper: %f, lower: %f)",
                 config::get_alliance_colour_name(config::get_instance()->get_alliance_colour()), upperHue, lowerHue);
    this->upperOptical.set_led_pwm(0);
    this->lowerOptical.set_led_pwm(0);
    this->motor.move_absolute(this->motor.get_position() - 25, 100);
  }
}

[[nodiscard]] bool Intake::is_engaged() const { return this->engaged; }

[[nodiscard]] bool Intake::is_red(double hue) { return hue < 30.0 || hue > 310.0; }
[[nodiscard]] bool Intake::is_lower_red(double hue) { return hue < 40.0 || hue > 310.0; }
[[nodiscard]] bool Intake::is_blue(double hue) { return hue < 250.0 && hue > 210.0; }
[[nodiscard]] bool Intake::is_lower_blue(double hue) { return hue < 280.0 && hue > 70.0; }

[[nodiscard]] const device::Motor &Intake::get_motor() const { return this->motor; }

[[nodiscard]] const device::Optical &Intake::get_upper_optical() const { return this->upperOptical; }
[[nodiscard]] const device::Optical &Intake::get_lower_optical() const { return this->lowerOptical; }
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
