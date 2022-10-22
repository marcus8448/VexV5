#include "robot/indexer.hpp"
#include "logger.hpp"

namespace robot {
Indexer::Indexer(pros::Motor *motor) : motor(motor) { motor->set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE); }

Indexer::~Indexer() {
  free(this->motor);
  this->motor = nullptr;
}

void Indexer::push() {
  if (this->state == CHARGED) {
    this->motor->move_absolute(90.0, 150);
    this->set_state(State::PUSHING);
  }
}

void Indexer::charge() {
  this->motor->move(-80);
  this->set_state(CHARGING);
}

void Indexer::cycle() {
  if (this->state != CHARGED) {
    this->charge();
    this->awaitReady();
  }
  this->push();
  this->awaitPush();
  this->charge();
  this->awaitReady();
}

void Indexer::awaitReady(int millis_timeout) {
  if (this->state == CHARGING) {
    millis_timeout /= 50;
    int i = 0;
    pros::delay(250);
    do {
      if (i++ == millis_timeout) {
        break;
      }
      pros::delay(50);
    } while (this->motor->get_efficiency() > 1.0);
    this->motor->move(0);
    this->motor->tare_position();
    this->set_state(State::CHARGED);
  }
}

void Indexer::awaitPush(int millis_timeout) {
  if (this->state == PUSHING) {
    millis_timeout /= 50;
    int i = 0;
    while (std::abs(this->motor->get_target_position() - this->motor->get_position()) > 3.0) {
      if (i++ == millis_timeout) {
        break;
      }
      pros::delay(50);
    }
    this->set_state(PUSHED);
  }
}

void Indexer::update(Controller *controller) {
  this->ticksInState++;
  if (controller->up_pressed()) {
    this->push();
  }
  if (this->state == PUSHING) {
    if (std::abs(this->motor->get_target_position() - this->motor->get_position()) < 3.0) {
      this->state = PUSHED;
      this->motor->move(0);
    }
  } else if (this->state == PUSHED) {
    this->charge();
  } else if (this->state == State::CHARGING) {
    if (this->ticksInState > 10) {
      if (this->motor->get_efficiency() < 1.0) {
        this->set_state(State::CHARGED);
        this->motor->tare_position();
        this->motor->move(0);
      }
    }
  }
}

[[nodiscard]] pros::Motor *Indexer::get_motor() const { return this->motor; }

void Indexer::set_state(Indexer::State value) {
  this->state = value;
  this->ticksInState = 0;
}

} // namespace robot
