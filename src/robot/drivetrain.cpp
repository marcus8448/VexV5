#include "robot/drivetrain.hpp"
#include "debug/logger.hpp"
#include "robot/device/motor.hpp"
#include "util.hpp"

#define JOYSTICK_MAX 127
#define ACCEPTABLE_ERROR 10.0
#define ACCEPTABLE_OFFSET 1.0
#define STABILIZE_MARGIN 10

namespace robot {
Drivetrain::Drivetrain(const uint8_t rightFront, const uint8_t leftFront, const uint8_t rightBack,
                       const uint8_t leftBack, const uint8_t inertial)
    : rightFront(device::Motor(rightFront, "Right Front", pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_COAST, false)),
      leftFront(device::Motor(leftFront, "Left Front", pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_COAST, true)),
      rightBack(device::Motor(rightBack, "Right Back", pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_COAST, false)),
      leftBack(device::Motor(leftBack, "Left Back", pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_BRAKE_COAST, true)),
      imu(device::Inertial(inertial, "IDrive")) {}

Drivetrain::~Drivetrain() = default;

bool Drivetrain::is_at_target() const { return this->timeAtTarget > STABILIZE_MARGIN; }

void Drivetrain::forwards(double distance, bool wait) {
  info("Moving forwards %fin.", distance);
  this->setTarget(DIRECT_MOVE);
  this->targetHeading = this->heading;
  this->targetLeft = util::in_to_e_units(distance);
  this->targetRight = util::in_to_e_units(distance);
  if (wait) {
    timer_start(move_forwards);
    await_move();
    timer_print(move_forwards);
  }
}

void Drivetrain::backwards(double distance, bool wait) {
  info("Moving backwards %fin.", distance);
  this->setTarget(DIRECT_MOVE);
  this->targetHeading = this->heading;
  this->targetLeft = util::in_to_e_units(-distance);
  this->targetRight = util::in_to_e_units(-distance);
  if (wait) {
    timer_start(move_backwards);
    await_move();
    timer_print(move_backwards);
  }
}

void Drivetrain::turn_right(double degrees, bool wait) {
  info("Turning right %f degrees.", degrees);
  this->setTarget(STATIC_TURN);
  this->targetHeading += degrees - floor(degrees / 360.0);
  if (wait) {
    timer_start(turn_right);
    await_move();
    timer_print(turn_right);
  }
}

void Drivetrain::turn_left(double degrees, bool wait) {
  info("Turning left %f degrees.", degrees);
  this->setTarget(STATIC_TURN);
  this->targetHeading -= degrees - floor(degrees / 360.0);
  if (wait) {
    timer_start(turn_left);
    await_move();
    timer_print(turn_left);
  }
}

void Drivetrain::await_move() const {
  while (this->timeAtTarget < STABILIZE_MARGIN) {
    pros::delay(ROBOT_TICK_RATE * (STABILIZE_MARGIN - this->timeAtTarget));
  }
}

void Drivetrain::updateTargeting(Controller *controller) {
  if (controller->down_pressed() == 1) {
    this->arcade = !this->arcade;
    controller->rumble("-");
  }

  this->setTarget(OPERATOR_DIRECT);
  if (this->arcade) {
    double joystickRotX = controller->right_stick_x();
    double joystickY = controller->left_stick_y();

    double left = (joystickY + joystickRotX) / JOYSTICK_MAX;
    double right = (joystickY - joystickRotX) / JOYSTICK_MAX;
    double max = std::max(right, left);
    right /= max;
    left /= max;
    this->powerRight = static_cast<int16_t>(right * MOTOR_MAX_MILLIVOLTS);
    this->powerLeft = static_cast<int16_t>(left * MOTOR_MAX_MILLIVOLTS);
  } else {
    this->powerRight = static_cast<int16_t>(controller->right_stick_y() / JOYSTICK_MAX * MOTOR_MAX_MILLIVOLTS);
    this->powerLeft = static_cast<int16_t>(controller->left_stick_y() / JOYSTICK_MAX * MOTOR_MAX_MILLIVOLTS);
  }
}

void Drivetrain::updateMovement() {
  bool atTarget = false;
  switch (this->targetType) {
  case NONE: {
    atTarget = true;
    break;
  }
  case OPERATOR_DIRECT: {
    if (this->powerRight == 0 && this->powerLeft == 0) {
      if (++this->timeOff == 200) {
        this->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        this->brake();
      }
    } else {
      this->set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
      this->timeOff = 0;
    }
    this->power_right(this->powerRight);
    this->power_left(this->powerLeft);
  }
  case STATIC_TURN: {
    double offset = this->targetHeading - this->heading;
    if (std::abs(offset) > 0.2) {
      if (offset > 180.0) {
        offset -= 360.0;
      } else if (offset < -180.0) {
        offset += 360;
      }

      double distance = util::turn_to_e_units(offset);
      this->move_right_targeting(this->rightPos - distance);
      this->move_left_targeting(this->leftPos + distance);
      if (std::abs(distance) < ACCEPTABLE_OFFSET && std::abs(distance) < ACCEPTABLE_OFFSET) {
        atTarget = true;
      }
    }
  }
  case DIRECT_MOVE: {
    this->move_right_targeting(this->targetLeft);
    this->move_left_targeting(this->targetRight);
    if (std::abs(this->targetLeft - this->leftPos) < ACCEPTABLE_OFFSET &&
        std::abs(this->targetRight - this->rightPos) < ACCEPTABLE_OFFSET) {
      atTarget = true;
    }
  }
  }
  if (atTarget) {
    this->timeAtTarget++;
  } else {
    this->timeAtTarget = 0;
  }
}

void Drivetrain::brake() {
  this->rightFront.brake();
  this->leftFront.brake();
  this->rightBack.brake();
  this->leftBack.brake();
}

void Drivetrain::set_brake_mode(pros::motor_brake_mode_e brake_mode) {
  this->leftFront.set_brake_mode(brake_mode);
  this->leftBack.set_brake_mode(brake_mode);
  this->rightFront.set_brake_mode(brake_mode);
  this->rightBack.set_brake_mode(brake_mode);
}

void Drivetrain::tare() {
  this->rightFront.tare();
  this->leftFront.tare();
  this->rightBack.tare();
  this->leftBack.tare();
  this->imu.tare();

  this->targetHeading -= this->heading;
  this->heading = 0.0;
}

void Drivetrain::move_right_targeting(double target) {
  this->prevErrorRight = this->errorRight;
  this->errorRight = target - this->rightPos;
  this->integralRight += this->errorRight;
  if (std::abs(this->errorRight) < ACCEPTABLE_ERROR) {
    this->integralRight = 0;
  }
  double derivative = this->errorRight - this->prevErrorRight;
  auto output =
      static_cast<int16_t>(this->errorLeft * this->kp + this->integralLeft * this->ki + derivative * this->kd);
  if (abs(output) > MOTOR_MAX_MILLIVOLTS) {
    output = MOTOR_MAX_MILLIVOLTS;
  }
  power_right(output);
}

void Drivetrain::move_left_targeting(double target) {
  this->prevErrorLeft = this->errorLeft;
  this->errorLeft = target - this->leftPos;
  this->integralLeft += this->errorLeft;
  if (std::abs(this->errorLeft) < ACCEPTABLE_ERROR) {
    this->integralLeft = 0;
  }
  double derivative = this->errorLeft - this->prevErrorLeft;
  auto output =
      static_cast<int16_t>(this->errorLeft * this->kp + this->integralLeft * this->ki + derivative * this->kd);
  if (abs(output) > MOTOR_MAX_MILLIVOLTS) {
    output = MOTOR_MAX_MILLIVOLTS;
  }
  power_left(output);
}

void Drivetrain::power_right(int16_t millivolts) {
  this->rightFront.move_millivolts(millivolts);
  this->rightBack.move_millivolts(millivolts);
}

void Drivetrain::power_left(int16_t millivolts) {
  this->leftFront.move_millivolts(millivolts);
  this->leftBack.move_millivolts(millivolts);
}

void Drivetrain::setTarget(Drivetrain::TargetType type) { this->targetType = type; }

void Drivetrain::updatePosition() {
  this->heading = this->imu.get_heading();
  this->rightPos = (this->rightFront.get_position() + this->rightBack.get_position()) / 2.0;
  this->leftPos = (this->leftFront.get_position() + this->leftBack.get_position()) / 2.0;
}
} // namespace robot
