#include "robot/drivetrain.hpp"
#include "debug/logger.hpp"
#include "pros/rtos.h"
#include "robot/device/motor.hpp"
#include "units.hpp"
#include <algorithm>

namespace robot {
constexpr double ACCEPTABLE_POSITION_ERROR = 5.0;
constexpr double ACCEPTABLE_HEADING_ERROR = 0.4;
constexpr uint16_t STABILIZE_TICKS = 25;
Drivetrain::Drivetrain(int8_t left1, int8_t left2, int8_t left3, int8_t right1, int8_t right2, int8_t right3,
                       int8_t inertial)
    : motorLeft(new device::MotorGroup<3>(
          {static_cast<int8_t>(-left1), static_cast<int8_t>(-left2), left3}, "Drive L", pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST)),
      motorRight(new device::MotorGroup<3>(
          {right1, right2, static_cast<int8_t>(-right3)}, "Drive R", pros::E_MOTOR_GEAR_BLUE, pros::E_MOTOR_BRAKE_COAST)),
      imu(device::Inertial(inertial, "IMU")), velRightPID(120.0, 15.0, 20.0, 50.0, 0.0),
      rightPID(6.0, 0.10, 1.5, 90.0, 3.0), headingPID(80.0, 15.0, 10.0, 10.0, 0.3) {}

bool Drivetrain::isAtTarget() const { return this->timeAtTarget > STABILIZE_TICKS; }

void Drivetrain::forwards(double distance, bool wait) {
  logger::info("Moving forwards %fin.", distance);
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
  logger::info("Moving backwards %fin.", distance);
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
  logger::info("Turning right %f degrees.", degrees);
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
  logger::info("Turning left %f degrees.", degrees);
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
  while (!this->isAtTarget()) {
    pros::c::delay(robot::device::TICK_RATE * (STABILIZE_TICKS - this->timeAtTarget));
  }
}

void Drivetrain::updateTargeting(control::input::Controller *controller) {
  this->setTarget(OPERATOR_DIRECT);
  if (this->controlScheme == ARCADE) {
    double power = controller->leftStickY();
    double rotation = controller->rightStickX() * 0.8;

    double left = (power + rotation) / control::input::Controller::JOYSTICK_MAX;
    double right = (power - rotation) / control::input::Controller::JOYSTICK_MAX;
    if (std::abs(right) > 1.0 || std::abs(left) > 1.0) {
      double max = std::max(std::abs(right), std::abs(left));
      right /= max;
      left /= max;
    }
    //    this->powerRight = this->velRightPID.update(right * 200.0, this->motorR1.getVelocity());
    //    this->powerLeft = this->velLeftPID.update(left * 200.0, this->motorL1.getVelocity());
    this->powerRight = static_cast<int16_t>(right * device::Motor::MAX_MILLIVOLTS);
    this->powerLeft = static_cast<int16_t>(left * device::Motor::MAX_MILLIVOLTS);
  } else {
    //    this->powerRight = this->velRightPID.update(controller->rightStickY() / JOYSTICK_MAX * 200.0,
    //    this->motorR1.getVelocity()); this->powerLeft = this->velLeftPID.update(controller->leftStickY() /
    //    JOYSTICK_MAX * 200.0, this->motorL1.getVelocity());
    this->powerRight = static_cast<int16_t>((controller->rightStickY() * 0.9) /
                                            control::input::Controller::JOYSTICK_MAX * device::Motor::MAX_MILLIVOLTS);
    this->powerLeft = static_cast<int16_t>((controller->leftStickY() * 0.9) / control::input::Controller::JOYSTICK_MAX *
                                           device::Motor::MAX_MILLIVOLTS);
  }
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

  logger::info("D %.2f /%.2f | P %.2f / %.2f | H %.2f", leftDiff, rightDiff, this->leftPos, this->rightPos,
               this->heading);
  // drivetrain: 12in x 11.5in
  // 860.40 = 90
  // 4.5in, 6.25in
  double avgHeadDeg = (this->heading + prevHeading) * M_PI / 360.0;

  double dLPosX = leftDiff * std::sin(avgHeadDeg);
  double dLPosY = leftDiff * std::cos(avgHeadDeg);
  lPosX += dLPosX;
  lPosY += dLPosY;
  double dRPosX = rightDiff * std::sin(avgHeadDeg);
  double dRPosY = rightDiff * std::cos(avgHeadDeg);
  rPosX += dRPosX;
  rPosY += dRPosY;

  double dist = std::sqrt(((lPosX - rPosX) * (lPosX - rPosX)) + ((lPosY - rPosY) * (lPosY - rPosY)));
//  logger::info("dist: %.2f", dist);

  this->posX = (lPosX + rPosX) / 2.0;
  this->posY = (lPosY + rPosY) / 2.0;
//  logger::info("L %.2f, %.2f | R %.2f, %.2f | C %.2f, %2.f", lPosX, lPosY, rPosX, rPosY, this->posX, this->posY);

  this->leftPID.copyParams(this->rightPID);
  this->velLeftPID.copyParams(this->velRightPID);

  bool atTarget = false;
  switch (this->targetType) {
  case NONE: {
    atTarget = true;
    break;
  }
  case OPERATOR_DIRECT: {
    if (this->powerRight == 0 && this->powerLeft == 0) {
      if (++this->timeOff == 200) {
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
    logger::info("%.2f/%.2f", this->heading, this->targetHeading);

      this->motorRight->moveMillivolts(static_cast<int16_t>(-value));
      this->motorLeft->moveMillivolts(static_cast<int16_t>(value));
    if (std::abs(this->targetHeading - this->heading) < ACCEPTABLE_HEADING_ERROR) {
      atTarget = true;
    }
    break;
  }
  case CURVE_MOVE: {
    //    double h = std::sqrt((this->posX - this->endCurveX) * (this->posX - this->endCurveX) +
    //                         (this->posY - this->endCurveY) * (this->posY - this->endCurveY));
    //    this->targetPosX = this->endCurveX - h * std::sin(this->curveAngle) * this->curve;
    //    this->targetPosY = this->endCurveY - h * std::cos(this->curveAngle) * this->curve;
  }
  case DIRECT_MOVE: {
    double right = this->rightPID.update(this->targetRight, this->rightPos);
    double left = this->leftPID.update(this->targetLeft, this->leftPos);
    double head = 0.0;//this->headingPID.update(this->targetHeading, this->heading);

    left += head / 2.0;
    right -= head / 2.0;
    if (left < -device::Motor::MAX_MILLIVOLTS) {
      right += -device::Motor::MAX_MILLIVOLTS - left;
    }
    if (right > device::Motor::MAX_MILLIVOLTS) {
      left += right - device::Motor::MAX_MILLIVOLTS;
    }

    this->motorRight->moveMillivolts(static_cast<int16_t>(right));
    this->motorLeft->moveMillivolts(static_cast<int16_t>(left));

    logger::info("L%.2f / R%.2f H%.2f", this->targetLeft - this->leftPos, this->targetRight - this->rightPos,
                 this->heading);
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
  this->motorLeft->brake();
  this->motorRight->brake();
}

void Drivetrain::setBrakeMode(pros::motor_brake_mode_e brake_mode) {
  this->motorLeft->setBrakeMode(brake_mode);
  this->motorRight->setBrakeMode(brake_mode);
}

void Drivetrain::tare() {
  this->targetRight -= this->rightPos;
  this->targetLeft -= this->leftPos;
  this->targetHeading -= this->heading;

  this->rightPos = 0;
  this->leftPos = 0;
  this->heading = 0.0;

  this->motorLeft->tare();
  this->motorRight->tare();
  this->imu.tare();
}

void Drivetrain::setTarget(Drivetrain::TargetType type) {
  this->targetType = type;
  this->timeAtTarget = 0;
}

double Drivetrain::getLeftVelocity() const {
  return this->motorLeft->getVelocity();
}

double Drivetrain::getRightVelocity() const {
  return this->motorRight->getVelocity();
}

device::Motor &Drivetrain::getLeftMotor() const { return *this->motorLeft; }

device::Motor &Drivetrain::getRightMotor() const { return *this->motorRight; }

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
