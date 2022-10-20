#include "robot/roller.hpp"
#include "logger.hpp"
#include "configuration.hpp"

namespace robot {

Roller::Roller(pros::Motor *motor, pros::Optical *optical) : motor(motor), optical(optical) {
  this->motor->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
  this->optical->disable_gesture();
}

Roller::~Roller() {
  free(this->motor);
  this->motor = nullptr;
}

[[nodiscard]] pros::Motor *Roller::get_motor() const { return this->motor; }

void Roller::update(Controller *controller) {
  if (controller->a_pressed()) {
    this->motor->move(50);
  } else {
    this->motor->move(0);
  }
}

void Roller::spin_until_colour(config::AllianceColour teamColour, uint32_t timeout) {
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
      if (i++ == timeout) break;
    }
  }
  case config::AllianceColour::BLUE: {
    while (hue < 220.0 || hue > 240.0) {
      this->motor->move(50);
      pros::delay(50);
      if (i++ == timeout) break;
    }
  }
  }
  motor->move(0);
}

bool Roller::looking_at_roller() {
  return this->optical->get_saturation() > 0.5;
}
} // namespace robot
