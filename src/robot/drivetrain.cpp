#include "robot/drivetrain.hpp"
#include "debug/logger.hpp"
#include "robot/device/motor.hpp"
#include "units.hpp"
#include "pros/rtos.h"
#include <algorithm>

#define JOYSTICK_MAX 127.0
#define ACCEPTABLE_POSITION_ERROR 5.0
#define ACCEPTABLE_HEADING_ERROR 0.4
#define STABILIZE_TICKS 25

namespace robot {
Drivetrain::Drivetrain(uint8_t leftFront, uint8_t rightFront, uint8_t leftBack, uint8_t rightBack, uint8_t inertial)
    : leftFrontMotor(device::Motor(leftFront, "Drive LF", false, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST)),
      rightFrontMotor(device::Motor(rightFront, "Drive RF", true, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST)),
      leftBackMotor(device::Motor(leftBack, "Drive LB", false, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST)),
      rightBackMotor(device::Motor(rightBack, "Drive RB", true, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST)),
      imu(device::Inertial(inertial, "IDrive")), rightPID(21.5, 6.0, 16.0, 100.0, 5.0),
      leftPID(21.5, 6.0, 16.0, 100.0, 5.0), headingPID(125.0, 20.0, 90.0, 10.0, 0.2) {}

bool Drivetrain::isAtTarget() const { return this->timeAtTarget > STABILIZE_TICKS; }

void Drivetrain::forwards(double distance, bool wait) {
  info("Moving forwards %fin.", distance);
  this->setTarget(DIRECT_MOVE);
  this->targetHeading = this->heading;
  this->targetLeft += units::inchToEncoder(distance);
  this->targetRight += units::inchToEncoder(distance);
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
  this->targetLeft -= units::inchToEncoder(distance);
  this->targetRight -= units::inchToEncoder(distance);
  if (wait) {
    startTiming(move_backwards);
    awaitMovement();
    endTiming(move_backwards);
  }
}

void Drivetrain::turnRight(double degrees, bool wait) {
  info("Turning right %f degrees.", degrees);
  this->setTarget(STATIC_TURN);
  this->headingPID.resetState();
  this->targetHeading += degrees;
  if (wait) {
    startTiming(turnRight);
    awaitMovement();
    endTiming(turnRight);
  }
}

void Drivetrain::turnLeft(double degrees, bool wait) {
  info("Turning left %f degrees.", degrees);
  this->setTarget(STATIC_TURN);
  this->headingPID.resetState();
  this->targetHeading -= degrees;
  if (wait) {
    startTiming(turnLeft);
    awaitMovement();
    endTiming(turnLeft);
  }
}

void Drivetrain::curveTargeting(double relInFwd, double relInLat, double curve, double relAngle) {
  this->setTarget(CURVE_MOVE);
  this->endCurveX = units::inchToEncoder(relInFwd);
  this->endCurveY = units::inchToEncoder(relInLat);
  this->curve = curve;
  this->curveAngle = this->heading + relAngle;
}

void Drivetrain::awaitMovement() const {
  while (this->timeAtTarget < STABILIZE_TICKS) {
    pros::c::delay(ROBOT_TICK_RATE * (STABILIZE_TICKS - this->timeAtTarget));
  }
}

void Drivetrain::updateTargeting(control::input::Controller *controller) {
  this->setTarget(OPERATOR_DIRECT);
  if (this->controlScheme == ARCADE) {
    double power = controller->leftStickY();
    double rotation = controller->rightStickX();

    double left = (power + rotation) / JOYSTICK_MAX;
    double right = (power - rotation) / JOYSTICK_MAX;
    if (std::abs(right) > 1.0 || std::abs(left) > 1.0) {
      double max = std::abs(std::max(right, left));
      right /= max;
      left /= max;
    }
    this->powerRight = static_cast<int16_t>(right * MOTOR_MAX_MILLIVOLTS);
    this->powerLeft = static_cast<int16_t>(left * MOTOR_MAX_MILLIVOLTS);
  } else {
    this->powerRight = static_cast<int16_t>(controller->rightStickY() / JOYSTICK_MAX * MOTOR_MAX_MILLIVOLTS);
    this->powerLeft = static_cast<int16_t>(controller->leftStickY() / JOYSTICK_MAX * MOTOR_MAX_MILLIVOLTS);
  }
}

void Drivetrain::updateState() {
  double prevHeading = this->heading;
  double prevRightPos = this->rightPos;
  double prevLeftPos = this->leftPos;
  this->heading = this->imu.getRotation();
  this->rightPos = (this->rightFrontMotor.getPosition() + this->rightBackMotor.getPosition()) / 2.0;
  this->leftPos = (this->leftFrontMotor.getPosition() + this->leftBackMotor.getPosition()) / 2.0;

  double rightDiff = this->rightPos - prevRightPos;
  double leftDiff = this->leftPos - prevLeftPos;

  double diff = (rightDiff + leftDiff) / 2.0;
  double avgHead = (this->heading + prevHeading) / 2.0;
  this->posY += diff * sin(avgHead);
  this->posX += diff * cos(avgHead);

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
    break;
  }
  case STATIC_TURN: {
    auto value = this->headingPID.update(this->targetHeading, this->heading);

    this->moveRight(static_cast<int16_t>(value));
    this->moveLeft(static_cast<int16_t>(-value));
    if (std::abs(this->targetHeading - this->heading) < ACCEPTABLE_HEADING_ERROR) {
      atTarget = true;
    }
    break;
  }
  case CURVE_MOVE: {
    double h = std::sqrt((this->posX - this->endCurveX) * (this->posX - this->endCurveX) +
                         (this->posY - this->endCurveY) * (this->posY - this->endCurveY));
    this->targetPosX = this->endCurveX - h * std::sin(this->curveAngle) * this->curve;
    this->targetPosY = this->endCurveY - h * std::cos(this->curveAngle) * this->curve;
  }
  case DIRECT_MOVE: {
    double head = this->headingPID.update(this->targetHeading, this->heading);
    double right = this->rightPID.update(this->targetRight, this->rightPos);
    double left = this->leftPID.update(this->targetLeft, this->leftPos);

    left -= head;
    right += head;
    if (left < -MOTOR_MAX_MILLIVOLTS) {
      right += -MOTOR_MAX_MILLIVOLTS - left;
    }
    if (right > MOTOR_MAX_MILLIVOLTS) {
      left += right - MOTOR_MAX_MILLIVOLTS;
    }

    this->moveRight(static_cast<int16_t>(right));
    this->moveLeft(static_cast<int16_t>(left));

    info("@ %f / %f", right, left);
    if (std::abs(this->targetLeft - this->leftPos) < ACCEPTABLE_POSITION_ERROR &&
        std::abs(this->targetRight - this->rightPos) < ACCEPTABLE_POSITION_ERROR) {
      atTarget = true;
    }
    break;
  }
  }
  if (atTarget) {
    this->timeAtTarget++;
  } else {
    this->timeAtTarget = 0;
  }
}

void Drivetrain::brake() {
  this->rightFrontMotor.brake();
  this->leftFrontMotor.brake();
  this->rightBackMotor.brake();
  this->leftBackMotor.brake();
}

void Drivetrain::setBrakeMode(pros::motor_brake_mode_e brake_mode) {
  this->leftFrontMotor.setBrakeMode(brake_mode);
  this->leftBackMotor.setBrakeMode(brake_mode);
  this->rightFrontMotor.setBrakeMode(brake_mode);
  this->rightBackMotor.setBrakeMode(brake_mode);
}

void Drivetrain::tare() {
  this->rightFrontMotor.tare();
  this->leftFrontMotor.tare();
  this->rightBackMotor.tare();
  this->leftBackMotor.tare();
  this->imu.tare();

  this->targetHeading -= this->heading;
  this->heading = 0.0;
}

void Drivetrain::moveRight(int16_t millivolts) {
  this->rightFrontMotor.moveMillivolts(millivolts);
  this->rightBackMotor.moveMillivolts(millivolts);
}

void Drivetrain::moveLeft(int16_t millivolts) {
  this->leftFrontMotor.moveMillivolts(millivolts);
  this->leftBackMotor.moveMillivolts(millivolts);
}

void Drivetrain::setTarget(Drivetrain::TargetType type) { this->targetType = type; }

[[nodiscard]] const char *driveSchemeName(Drivetrain::ControlScheme scheme) {
  switch (scheme) {
  case Drivetrain::ControlScheme::TANK:
    return "Tank";
  case Drivetrain::ControlScheme::ARCADE:
    return "Arcade";
  }
  return "";
}
} // namespace robot
