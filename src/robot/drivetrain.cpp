#include "robot/drivetrain.hpp"
#include "debug/logger.hpp"
#include "pros/rtos.h"
#include "robot/device/motor.hpp"
#include "units.hpp"
#include <algorithm>

#define JOYSTICK_MAX 127.0
#define ACCEPTABLE_POSITION_ERROR 5.0
#define ACCEPTABLE_HEADING_ERROR 0.4
#define STABILIZE_TICKS 25

namespace robot {
Drivetrain::Drivetrain(uint8_t left1, uint8_t left2, uint8_t left3, uint8_t right1, uint8_t right2, uint8_t right3, uint8_t inertial)
    : motorL1(device::Motor(left1, "Drive L1", true, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST)),
      motorL2(device::Motor(left2, "Drive L2", true, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST)),
      motorL3(device::Motor(left3, "Drive L3", false, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST)),
      motorR1(device::Motor(right1, "Drive R1", false, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST)),
      motorR2(device::Motor(right2, "Drive R2", false, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST)),
      motorR3(device::Motor(right3, "Drive R3", true, pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST)),
      imu(device::Inertial(inertial, "IMU")), rightPID(12.0, 3.0, 3.0, 90.0, 5.0),
      leftPID(0.0, 0.0, 0.0, 0.0, 0.0), headingPID(80.0, 10.0, 0.0, 10.0, 10000.2) {}

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
    double rotation = controller->rightStickX() * 0.5;

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
    this->powerRight = static_cast<int16_t>((controller->rightStickY() * 0.8) / JOYSTICK_MAX * MOTOR_MAX_MILLIVOLTS);
    this->powerLeft = static_cast<int16_t>((controller->leftStickY() * 0.8) / JOYSTICK_MAX * MOTOR_MAX_MILLIVOLTS);
  }
}

void Drivetrain::updateState() {
  double prevHeading = this->heading;
  double prevRightPos = this->rightPos;
  double prevLeftPos = this->leftPos;
  this->heading = this->imu.getRotation();
  this->rightPos = (this->motorR1.getPosition() + this->motorR2.getPosition() + this->motorR3.getPosition()) / 3.0;
  this->leftPos = (this->motorL1.getPosition() + this->motorL2.getPosition() + this->motorL3.getPosition()) / 3.0;

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
//        this->setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
//        this->brake();
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
    this->leftPID.kp = this->rightPID.kp;
    this->leftPID.ki = this->rightPID.ki;
    this->leftPID.kd = this->rightPID.kd;
    this->leftPID.integralRange = this->rightPID.integralRange;
    this->leftPID.acceptableError = this->rightPID.acceptableError;
    this->leftPID.moveMin = this->rightPID.moveMin = 500;
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

    info("L%.2f / R%.2f H%.2f", this->targetLeft - this->leftPos, this->targetRight - this->rightPos, this->heading);
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
  this->motorL1.brake();
  this->motorL2.brake();
  this->motorL3.brake();
  this->motorR1.brake();
  this->motorR2.brake();
  this->motorR3.brake();
}

void Drivetrain::setBrakeMode(pros::motor_brake_mode_e brake_mode) {
  this->motorL1.setBrakeMode(brake_mode);
  this->motorL2.setBrakeMode(brake_mode);
  this->motorL3.setBrakeMode(brake_mode);
  this->motorR1.setBrakeMode(brake_mode);
  this->motorR2.setBrakeMode(brake_mode);
  this->motorR3.setBrakeMode(brake_mode);
}

void Drivetrain::tare() {
  this->targetRight -= this->rightPos;
  this->targetLeft -= this->leftPos;
  this->targetHeading -= this->heading;

  this->rightPos = 0;
  this->leftPos = 0;
  this->heading = 0.0;

  this->motorL1.tare();
  this->motorL2.tare();
  this->motorL3.tare();
  this->motorR1.tare();
  this->motorR2.tare();
  this->motorR3.tare();
  this->imu.tare();
}

void Drivetrain::moveRight(int16_t millivolts) {
  this->motorR1.moveMillivolts(millivolts);
  this->motorR2.moveMillivolts(millivolts);
  this->motorR3.moveMillivolts(millivolts);
}

void Drivetrain::moveLeft(int16_t millivolts) {
  this->motorL1.moveMillivolts(millivolts);
  this->motorL2.moveMillivolts(millivolts);
  this->motorL3.moveMillivolts(millivolts);
}

void Drivetrain::setTarget(Drivetrain::TargetType type) {
  this->targetType = type;
  this->timeAtTarget = 0;
}

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
