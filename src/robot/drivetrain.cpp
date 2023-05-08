#include "robot/drivetrain.hpp"
#include "debug/logger.hpp"
#include "robot/device/motor.hpp"
#include "units.hpp"

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

bool Drivetrain::isAtTarget() const { return this->timeAtTarget > STABILIZE_MARGIN; }

void Drivetrain::forwards(double distance, bool wait) {
  info("Moving forwards %fin.", distance);
  this->setTarget(DIRECT_MOVE);
  this->targetHeading = this->heading;
  this->targetLeft = units::inchToEncoder(distance);
  this->targetRight = units::inchToEncoder(distance);
  if (wait) {
    startTiming(move_forwards);
    awaitMovement();
    endTiming(move_forwards);
  }
}

void Drivetrain::backwards(double distance, bool wait) {
  info("Moving backwards %fin.", distance);
  this->setTarget(DIRECT_MOVE);
  this->targetHeading = this->heading;
  this->targetLeft = units::inchToEncoder(-distance);
  this->targetRight = units::inchToEncoder(-distance);
  if (wait) {
    startTiming(move_backwards);
    awaitMovement();
    endTiming(move_backwards);
  }
}

void Drivetrain::turnRight(double degrees, bool wait) {
  info("Turning right %f degrees.", degrees);
  this->setTarget(STATIC_TURN);
  this->targetHeading += degrees - floor(degrees / 360.0);
  if (wait) {
    startTiming(turnRight);
    awaitMovement();
    endTiming(turnRight);
  }
}

void Drivetrain::turnLeft(double degrees, bool wait) {
  info("Turning left %f degrees.", degrees);
  this->setTarget(STATIC_TURN);
  this->targetHeading -= degrees - floor(degrees / 360.0);
  if (wait) {
    startTiming(turnLeft);
    awaitMovement();
    endTiming(turnLeft);
  }
}

void Drivetrain::awaitMovement() const {
  while (this->timeAtTarget < STABILIZE_MARGIN) {
    pros::delay(ROBOT_TICK_RATE * (STABILIZE_MARGIN - this->timeAtTarget));
  }
}

void Drivetrain::updateTargeting(control::input::Controller *controller) {
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
        this->setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
        this->brake();
      }
    } else {
      this->setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
      this->timeOff = 0;
    }
    this->moveRight(this->powerRight);
    this->moveLeft(this->powerLeft);
  }
  case STATIC_TURN: {
    double offset = this->targetHeading - this->heading;
    if (std::abs(offset) > 0.2) {
      if (offset > 180.0) {
        offset -= 360.0;
      } else if (offset < -180.0) {
        offset += 360;
      }

      double distance = units::degreesToEncoder(offset);
      this->moveRightTargeting(this->rightPos - distance);
      this->moveLeftTargeting(this->leftPos + distance);
      if (std::abs(distance) < ACCEPTABLE_OFFSET && std::abs(distance) < ACCEPTABLE_OFFSET) {
        atTarget = true;
      }
    }
  }
  case DIRECT_MOVE: {
    this->moveRightTargeting(this->targetLeft);
    this->moveLeftTargeting(this->targetRight);
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

void Drivetrain::setBrakeMode(pros::motor_brake_mode_e brake_mode) {
  this->leftFront.setBrakeMode(brake_mode);
  this->leftBack.setBrakeMode(brake_mode);
  this->rightFront.setBrakeMode(brake_mode);
  this->rightBack.setBrakeMode(brake_mode);
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

void Drivetrain::moveRightTargeting(double target) {
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
  moveRight(output);
}

void Drivetrain::moveLeftTargeting(double target) {
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
  moveLeft(output);
}

void Drivetrain::moveRight(int16_t millivolts) {
  this->rightFront.moveMillivolts(millivolts);
  this->rightBack.moveMillivolts(millivolts);
}

void Drivetrain::moveLeft(int16_t millivolts) {
  this->leftFront.moveMillivolts(millivolts);
  this->leftBack.moveMillivolts(millivolts);
}

void Drivetrain::setTarget(Drivetrain::TargetType type) { this->targetType = type; }

void Drivetrain::updatePosition() {
  this->heading = this->imu.getHeading();
  this->rightPos = (this->rightFront.getPosition() + this->rightBack.getPosition()) / 2.0;
  this->leftPos = (this->leftFront.getPosition() + this->leftBack.getPosition()) / 2.0;
}
} // namespace robot
