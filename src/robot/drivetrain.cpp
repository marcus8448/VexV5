#include "drivetrain.hpp"
#include "config/tuning.hpp"
#include "debug/logger.hpp"
#include "pros/rtos.h"
#include "robot/device/motor.hpp"
#include "units.hpp"
#include <algorithm>
#include <cmath>

namespace robot {
constexpr double ACCEPTABLE_HEADING_ERROR = 0.4;

Drivetrain::Drivetrain(const int8_t left1, const int8_t left2, const int8_t left3, const int8_t right1,
                       const int8_t right2, const int8_t right3, const int8_t inertial)
    : motorLeft(new device::MotorGroup<3>({static_cast<int8_t>(-left1), static_cast<int8_t>(-left2), left3}, "Drive L",
                                          pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST)),
      motorRight(new device::MotorGroup<3>({right1, right2, static_cast<int8_t>(-right3)}, "Drive R",
                                           pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST)),
      imu(device::Inertial(inertial, "IMU")), velRightPID(drivetrainKp, drivetrainKi, drivetrainKd, 50.0, 20.0),
      rightPID(drivetrainKp, drivetrainKi, drivetrainKd, 512.0, 3.0),
      headingPID(drivetrainHeadingKp, drivetrainHeadingKi, drivetrainHeadingKd, 10.0, 0.3) {}

bool Drivetrain::isAtTarget() const { return this->timeAtTarget >= this->stabilizeTicks; }

void Drivetrain::forwards(double distance, const int16_t limit, const bool wait) {
  logger::info("Moving forwards {}in.", distance);
  this->setTarget(DIRECT_MOVE);
  this->targetHeading = this->heading;
  this->targetLeft = this->leftPos + units::inchToEncoder(distance);
  this->targetRight = this->rightPos + units::inchToEncoder(distance);
  this->powerLimit = limit;
  if (wait) {
    startTiming(move_forwards);
    awaitMovement();
    endTiming(move_forwards);
  }
}

void Drivetrain::backwards(double distance, const int16_t limit, const bool wait) {
  logger::info("Moving backwards {}in.", distance);
  this->setTarget(DIRECT_MOVE);
  this->targetHeading = this->heading;
  this->targetLeft = this->leftPos - units::inchToEncoder(distance);
  this->targetRight = this->rightPos - units::inchToEncoder(distance);
  this->powerLimit = limit;
  if (wait) {
    startTiming(move_backwards);
    awaitMovement();
    endTiming(move_backwards);
  }
}

void Drivetrain::turnAbs(double degrees, const int16_t limit, const bool wait) {
  logger::info("Turning to {} degrees.", degrees);
  this->setTarget(STATIC_TURN);
  this->headingPID.resetState();
  this->targetHeading = degrees;
  this->powerLimit = limit;
  if (wait) {
    startTiming(turnAbs);
    awaitMovement();
    endTiming(turnAbs);
  }
}

void Drivetrain::turnRight(double degrees, const int16_t limit, const bool wait) {
  logger::info("Turning right {} degrees.", degrees);
  this->setTarget(STATIC_TURN);
  this->headingPID.resetState();
  this->targetHeading += degrees;
  this->powerLimit = limit;
  if (wait) {
    startTiming(turnRight);
    awaitMovement();
    endTiming(turnRight);
  }
}

void Drivetrain::turnLeft(double degrees, const int16_t limit, const bool wait) {
  logger::info("Turning left {} degrees.", degrees);
  this->setTarget(STATIC_TURN);
  this->headingPID.resetState();
  this->targetHeading -= degrees;
  this->powerLimit = limit;
  if (wait) {
    startTiming(turnLeft);
    awaitMovement();
    endTiming(turnLeft);
  }
}

void Drivetrain::pivotRight(double degrees, const int16_t limit, const bool wait) {
  logger::info("Pivot right {} degrees.", degrees);
  this->setTarget(PIVOT_TURN_RIGHT);
  this->headingPID.resetState();
  this->targetHeading += degrees;
  this->powerLimit = limit;
  if (wait) {
    startTiming(pivotRight);
    awaitMovement();
    endTiming(pivotRight);
  }
}

void Drivetrain::pivotLeft(double degrees, const int16_t limit, const bool wait) {
  logger::info("Pivot left {} degrees.", degrees);
  this->setTarget(PIVOT_TURN_LEFT);
  this->headingPID.resetState();
  this->targetHeading -= degrees;
  this->powerLimit = limit;
  if (wait) {
    startTiming(pivotLeft);
    awaitMovement();
    endTiming(pivotLeft);
  }
}

void Drivetrain::pivotRightAbs(double degrees, const int16_t limit, const bool wait) {
  logger::info("Pivot absolute right {} degrees.", degrees);
  this->setTarget(PIVOT_TURN_RIGHT);
  this->headingPID.resetState();
  this->targetHeading = degrees;
  this->powerLimit = limit;
  if (wait) {
    startTiming(pivotRight);
    awaitMovement();
    endTiming(pivotRight);
  }
}

void Drivetrain::pivotLeftAbs(double degrees, const int16_t limit, const bool wait) {
  logger::info("Pivot left absolute to {} degrees.", degrees);
  this->setTarget(PIVOT_TURN_LEFT);
  this->headingPID.resetState();
  this->targetHeading = degrees;
  this->powerLimit = limit;
  if (wait) {
    startTiming(pivotLeft);
    awaitMovement();
    endTiming(pivotLeft);
  }
}

void Drivetrain::awaitMovement() const {
  while (!this->isAtTarget()) {
    pros::c::delay(device::TICK_RATE * (this->stabilizeTicks - this->timeAtTarget));
  }
}

void Drivetrain::manualOverride() {
  this->targetType = NONE;
}

void Drivetrain::updateTargeting(control::input::Controller *controller) {
  this->setTarget(OPERATOR_DIRECT);
  this->velLeftPID = this->velRightPID;

  if (this->controlScheme == ARCADE) {
    double power = controller->leftStickY();
    double rotation = controller->rightStickX() * arcadeTurnMultiplier;

    double left = (power + rotation) / control::input::Controller::JOYSTICK_MAX;
    double right = (power - rotation) / control::input::Controller::JOYSTICK_MAX;
    if (std::abs(right) > 1.0 || std::abs(left) > 1.0) {
      double max = std::max(std::abs(right), std::abs(left));
      right /= max;
      left /= max;
    }
    this->powerRight = static_cast<int16_t>(right * this->operatorPower);
    this->powerLeft = static_cast<int16_t>(left * this->operatorPower);
  } else {
    double right =
        controller->rightStickY() * tankMoveMultiplier / control::input::Controller::JOYSTICK_MAX * this->operatorPower;
    double left =
        controller->leftStickY() * tankMoveMultiplier / control::input::Controller::JOYSTICK_MAX * this->operatorPower;
    if (left == 0.0 || right == 0.0) {
      left *= 0.8;
      right *= 0.8;
    } else if (std::signbit(left) == std::signbit(right)) {
      if (std::abs(right) > 50.0 && std::abs(left) > 50.0) {
        double delta = right - left;
        right -= delta / 4.0;
        left += delta / 4.0;
      }
    } else {
      right *= 0.6;
      left *= 0.6;
    }
    this->powerRight = static_cast<int16_t>(right);
    this->powerLeft = static_cast<int16_t>(left);

    //    if (right != 0.0) {
    //      this->powerRight = static_cast<int16_t>(
    //          this->velRightPID.update((controller->rightStickY() / control::input::Controller::JOYSTICK_MAX) * 600.0,
    //                                   this->motorRight->getVelocity()));
    //    }
    //    if (left != 0.0) {
    //      this->powerLeft = static_cast<int16_t>(
    //          this->velLeftPID.update((controller->leftStickY() / control::input::Controller::JOYSTICK_MAX) * 600.0,
    //                                  this->motorLeft->getVelocity()));
    //    }
  }
  logger::debug("{}mV / {}mV", this->powerLeft, this->powerRight);
}

void Drivetrain::updateState() {
  double prevHeading = this->heading;
  double prevRightPos = this->rightPos;
  double prevLeftPos = this->leftPos;
  this->heading = this->imu.getRotation();
  this->rightPos = this->motorRight->getPosition();
  this->leftPos = this->motorLeft->getPosition();

  double rightDiff = units::encoderToInch(this->rightPos - prevRightPos);
  double leftDiff = units::encoderToInch(this->leftPos - prevLeftPos);

  //  logger::info("D {:.2f} /{:.2f} | P {:.2f} / {:.2f} | H {:.2f}", leftDiff, rightDiff, this->leftPos,
  //  this->rightPos,
  //               this->heading);
  // drivetrain: 12in x 11.5in
  // 860.40 = 90
  // 4.5in, 6.25in
  double avgHeadDeg = (this->heading + prevHeading) * std::numbers::pi / 360.0;

  double dLPosX = leftDiff * std::sin(avgHeadDeg);
  double dLPosY = leftDiff * std::cos(avgHeadDeg);
  this->lPosX += dLPosX;
  this->lPosY += dLPosY;
  double dRPosX = rightDiff * std::sin(avgHeadDeg);
  double dRPosY = rightDiff * std::cos(avgHeadDeg);
  this->rPosX += dRPosX;
  this->rPosY += dRPosY;

  // double dist = std::sqrt(((lPosX - rPosX) * (lPosX - rPosX)) + ((lPosY - rPosY) * (lPosY - rPosY)));
  //  logger::info("dist: {:.2f}", dist);

  this->posX = (this->lPosX + this->rPosX) / 2.0;
  this->posY = (this->lPosY + this->rPosY) / 2.0;
  //  logger::info("L {:.2f}, {:.2f} | R {:.2f}, {:.2f} | C {:.2f}, {:.2f}", lPosX, lPosY, rPosX, rPosY, this->posX,
  //  this->posY);

  this->leftPID = this->rightPID;

  bool atTarget = false;
  switch (this->targetType) {
  case NONE: {
    atTarget = true;
    break;
  }
  case OPERATOR_DIRECT: {
    if (this->powerRight == 0 && this->powerLeft == 0) {
      if (++this->timeOff >= 8) {
        this->setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
        this->brake();
      }
    } else {
      this->setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
      this->timeOff = 0;
    }
    this->motorRight->moveMillivolts(this->powerRight);
    this->motorLeft->moveMillivolts(this->powerLeft);
    break;
  }
  case STATIC_TURN: {
    auto value = this->headingPID.update(this->targetHeading, this->heading);
    if (value > this->powerLimit) {
      value = this->powerLimit;
    } else if (value < -this->powerLimit) {
      value = -this->powerLimit;
    }
    //    logger::info("{:.2f}/{:.2f}", this->heading, this->targetHeading);

    this->motorRight->moveMillivolts(static_cast<int16_t>(-value));
    this->motorLeft->moveMillivolts(static_cast<int16_t>(value));
    if (std::abs(this->targetHeading - this->heading) < ACCEPTABLE_HEADING_ERROR) {
      atTarget = true;
    }
    break;
  }
  case PIVOT_TURN_RIGHT: {
    auto value = this->headingPID.update(this->targetHeading, this->heading);
    if (value > this->powerLimit) {
      value = this->powerLimit;
    } else if (value < -this->powerLimit) {
      value = -this->powerLimit;
    }
    //    logger::info("{:.2f}/{:.2f}", this->heading, this->targetHeading);

    this->motorLeft->moveMillivolts(static_cast<int16_t>(value * 1.5));
    if (std::abs(this->targetHeading - this->heading) < ACCEPTABLE_HEADING_ERROR) {
      atTarget = true;
    }
    break;
  }
  case PIVOT_TURN_LEFT: {
    auto value = this->headingPID.update(this->targetHeading, this->heading);
    if (value > this->powerLimit) {
      value = this->powerLimit;
    } else if (value < -this->powerLimit) {
      value = -this->powerLimit;
    }
    //    logger::info("{:.2f}/{:.2f}", this->heading, this->targetHeading);

    this->motorRight->moveMillivolts(static_cast<int16_t>(-value * 1.5));
    if (std::abs(this->targetHeading - this->heading) < ACCEPTABLE_HEADING_ERROR) {
      atTarget = true;
    }
    break;
  }
  case DIRECT_MOVE: {
    double right = this->rightPID.update(this->targetRight, this->rightPos);
    double left = this->leftPID.update(this->targetLeft, this->leftPos);
    double head = this->headingPID.update(this->targetHeading, this->heading) / 4.0;

    if (this->rightPID.getError() > 360.0 * 2 && this->leftPID.getError() > 360.0 * 2) {
      left += head;
      right -= head;
    }

    if (left > this->powerLimit || right > this->powerLimit || left < -this->powerLimit || right < -this->powerLimit) {
      const double maximum = std::max(std::abs(left), std::abs(right));
      left = left / maximum * this->powerLimit;
      right = right / maximum * this->powerLimit;
    }

    this->motorRight->moveMillivolts(static_cast<int16_t>(right));
    this->motorLeft->moveMillivolts(static_cast<int16_t>(left));

    //    logger::info("L{:.2f} / R{:.2f} H{:.2f}", this->targetLeft - this->leftPos, this->targetRight -
    //    this->rightPos,
    //                 this->heading);
    if (std::abs(this->targetLeft - this->leftPos) < this->acceptablePositionError &&
        std::abs(this->targetRight - this->rightPos) < this->acceptablePositionError) {
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
  this->motorLeft->brake();
  this->motorRight->brake();
}

void Drivetrain::setBrakeMode(const pros::motor_brake_mode_e brake_mode) {
  this->motorLeft->setBrakeMode(brake_mode);
  this->motorRight->setBrakeMode(brake_mode);
}

void Drivetrain::tare() {
  this->targetRight -= this->rightPos;
  this->targetLeft -= this->leftPos;
  this->targetHeading -= this->heading;

  this->rightPos = 0.0;
  this->leftPos = 0.0;
  this->heading = 0.0;

  this->posX = 0.0;
  this->posY = 0.0;

  this->rPosX = +6.25;
  this->rPosY = 0.0;
  this->lPosX = -6.25;
  this->lPosY = 0.0;

  this->motorLeft->tare();
  this->motorRight->tare();
  this->imu.tare();
}

void Drivetrain::setTarget(const TargetType type) {
  this->targetType = type;
  this->timeAtTarget = 0;
}

double Drivetrain::getLeftVelocity() const { return this->motorLeft->getVelocity(); }

double Drivetrain::getRightVelocity() const { return this->motorRight->getVelocity(); }

device::Motor &Drivetrain::getLeftMotor() const { return *this->motorLeft; }

device::Motor &Drivetrain::getRightMotor() const { return *this->motorRight; }

[[nodiscard]] const char *driveSchemeName(const Drivetrain::ControlScheme scheme) {
  switch (scheme) {
  case Drivetrain::ControlScheme::TANK:
    return "Tank";
  case Drivetrain::ControlScheme::ARCADE:
    return "Arcade";
  }
  return "";
}
} // namespace robot
