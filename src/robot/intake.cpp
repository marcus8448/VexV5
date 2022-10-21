#include "robot/intake.hpp"
#include "pros/optical.hpp"
#include "logger.hpp"

namespace robot {

Intake::Intake(pros::Motor *motor, pros::Optical *optical) : motor(motor), optical(optical) {
  this->motor->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  this->optical->disable_gesture();
}

Intake::~Intake() {
  free(this->motor);
  this->motor = nullptr;
  free(this->optical);
  this->optical = nullptr;
}

void Intake::engage() {
  this->motor->move(-127);
  this->engaged = true;
}

void Intake::reverse() {
  this->motor->move(127);
  this->engaged = true;
}

void Intake::disengage() {
  this->engaged = false;
  this->motor->brake();
}

void Intake::spin_until_colour(config::AllianceColour teamColour, uint32_t timeout) {
  if (!looking_at_roller()) {
    logger::warn("Spinning roller while not in view?");
  }
  double hue = this->optical->get_hue();
  uint32_t i = 0;
  timeout /= 50;
  switch (teamColour) {
  case config::AllianceColour::RED: {
    while (hue < 335.0 || hue > 15.0) {
      this->motor->move(50);
      pros::delay(50);
      if (i++ == timeout)
        break;
    }
  }
  case config::AllianceColour::BLUE: {
    while (hue < 220.0 || hue > 240.0) {
      this->motor->move(50);
      pros::delay(50);
      if (i++ == timeout)
        break;
    }
  }
  }
  motor->move(0);
}

bool Intake::looking_at_roller() { return this->optical->get_saturation() > 0.5; }

[[nodiscard]] bool Intake::isEngaged() const { return this->engaged; }

[[nodiscard]] pros::Motor *Intake::get_motor() const { return this->motor; }

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
