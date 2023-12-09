#include "catapult.hpp"

#include "debug/logger.hpp"

namespace robot {
Catapult::Catapult(int8_t motorPort, int8_t motor2Port, int8_t rotationPort)
    : motor(device::DirectMotor(motorPort, "Catapult1", true, pros::E_MOTOR_GEAR_RED)),
      motor2(device::DirectMotor(motor2Port, "Catapult2", false, pros::E_MOTOR_GEAR_RED)),
      rotation(device::Rotation(rotationPort, "CatapultR")), pid(1.8, 0.0006, 0.3, 13000.0, 2000.0) {}

void Catapult::launch(const uint16_t count, const int16_t speed, uint16_t delay, const bool wait) {
  this->pendingLaunches = count;
  this->targetTime = 0;
  this->delay = delay;
  this->speed = speed;
  this->pid.resetState();
  if (wait) {
    while (this->pendingLaunches > 0) {
      pros::c::delay(50);
    }
  }
}

void Catapult::hold() {
  this->pendingLaunches = 0;
  this->speed = 0;
  this->delay = 0;
  this->targetTime = 0;
  this->pid.resetState();
}

int16_t Catapult::getSpeed() const { return this->speed; }

void Catapult::updateTargeting(control::input::Controller *controller) {
  if (controller->l1Pressed()) {
    this->launch(1, device::Motor::MAX_MILLIVOLTS);
  } else if (controller->l2Pressed()) {
    this->hold();
  } else if (controller->leftPressed()) {
    // this->launch(1000, 6000);
    this->launch(30, 12000, 500);
}
}

void Catapult::updateState() {
  if (!this->prime) {
    this->motor.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    this->motor2.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
  } else {
    this->motor.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
    this->motor2.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
  }

  if (this->position == CHARGE) {
    if (this->rotation.getRotation() > 8000) {
      this->position = RELEASE;
      if (this->pendingLaunches > 0) {
        this->pendingLaunches--;
      }
    }
  } else {
    if (this->rotation.getRotation() < 7500) {
      this->position = CHARGE;
      this->targetTime = pros::c::millis() + this->delay;
    }
  }
  if (this->pendingLaunches == 0 || pros::c::millis() < this->targetTime) {
    const double update = -this->pid.update(4500, this->rotation.getRotation());
    if (update == 0.0 || !prime) {
      this->motor.brake();
      this->motor2.brake();
    } else {
      this->motor.moveMillivolts(static_cast<int16_t>(update));
      this->motor2.moveMillivolts(static_cast<int16_t>(update));
    }
  } else {
    this->motor.moveMillivolts(this->speed);
    this->motor2.moveMillivolts(this->speed);
  }
}
} // namespace robot
