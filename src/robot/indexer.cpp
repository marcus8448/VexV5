#include "robot/indexer.hpp"
#include "logger.hpp"
#include "robot/device/motor.hpp"

namespace robot {
Indexer::Indexer(uint8_t port)
    : motor(device::Motor(port, pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_BRAKE, false)) {}

Indexer::~Indexer() = default;

void Indexer::push() {
  if (this->state == CHARGED) {
    logger::info("Shooting.");
    this->motor.move_absolute(90.0, 200);
    this->set_state(State::PUSHING);
  }
}

void Indexer::charge() {
  this->motor.move_millivolts(-7200);
  this->set_state(CHARGING);
}

void Indexer::cycle() {
  if (this->state != CHARGED) {
    this->charge();
    this->await_ready();
  }
  this->push();
  this->await_push();
  this->charge();
  this->await_ready();
}

void Indexer::await_ready(int16_t timeout_millis) {
  if (this->state == CHARGING) {
    timeout_millis /= 50;
    uint16_t i = 0;
    pros::delay(250);
    do {
      if (i++ == timeout_millis) {
        break;
      }
      pros::delay(50);
    } while (this->motor.get_efficiency() > 1.0);
    this->motor.stop();
    this->motor.tare();
    this->set_state(State::CHARGED);
  }
}

void Indexer::await_push(int16_t timeout_millis) {
  if (this->state == PUSHING) {
    this->motor.await_target(timeout_millis);
    this->set_state(PUSHED);
  }
}

void Indexer::update(Controller *controller) {
  this->ticksInState++;
  if (controller->r2_pressed()) {
    this->push();
  }
  if (this->state == PUSHING) {
    this->motor.move_absolute(90.0, 200);
    if (std::abs(90.0 - this->motor.get_position()) < 3.0) {
      this->state = PUSHED;
      this->charge();
    }
  } else if (this->state == PUSHED) {
    this->charge();
  } else if (this->state == State::CHARGING) {
    if (this->ticksInState > 15) {
      if (this->motor.get_efficiency() < 1.0) {
        this->set_state(State::CHARGED);
        this->motor.tare();
        this->motor.stop();
      }
    }
  }
}

[[nodiscard]] const device::Motor &Indexer::get_motor() const { return this->motor; }

void Indexer::set_state(Indexer::State value) {
  this->state = value;
  this->ticksInState = 0;
}

} // namespace robot
