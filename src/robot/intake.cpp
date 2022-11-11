#include "robot/intake.hpp"
#include "pros/optical.hpp"
#include "robot/device/motor.hpp"

namespace robot {

Intake::Intake(uint8_t port) : motor(device::Motor(port, pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_BRAKE, true)) {}

Intake::~Intake() = default;

void Intake::engage(int16_t target_millivolts) {
  this->motor.set_reversed(false);
  this->motor.move_millivolts(target_millivolts);
  this->engaged = true;
}

void Intake::reverse(int16_t target_millivolts) {
  this->motor.set_reversed(true);
  this->motor.move_millivolts(target_millivolts);
  this->engaged = true;
}

void Intake::disengage() {
  this->engaged = false;
  this->motor.stop();
}

void Intake::roll_to_team_colour(config::AllianceColour teamColour, uint32_t timeout) {
  this->reverse(4800);
  pros::delay(750);
  this->disengage();
  //  if (!looking_at_roller()) {
  //    logger::warn("Spinning roller while not in view?");
  //  }
  //  double hue = this->optical->get_hue();
  //  uint32_t i = 0;
  //  timeout /= 50;
  //  switch (teamColour) {
  //  case config::AllianceColour::RED: {
  //    while (hue < 335.0 || hue > 15.0) {
  //      this->motor->move(50);
  //      pros::delay(50);
  //      if (i++ == timeout)
  //        break;
  //    }
  //  }
  //  case config::AllianceColour::BLUE: {
  //    while (hue < 220.0 || hue > 240.0) {
  //      this->motor->move(50);
  //      pros::delay(50);
  //      if (i++ == timeout)
  //        break;
  //    }
  //  }
  //  }
  //  motor->move(0);
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
