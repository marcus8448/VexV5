#include "wings.hpp"

namespace robot {
Wings::Wings(int8_t leftPneumatic, int8_t rightPneumatic)
    : left(device::PneumaticPiston(leftPneumatic, "Wing L")), right(device::PneumaticPiston(rightPneumatic, "Wing R")) {
}

void Wings::open() {
  this->openLeft();
  this->openRight();
}

void Wings::openLeft() {
  if (!this->leftOpen) {
    this->left.extend();
    this->leftOpen = true;
  }
}

void Wings::openRight() {
  if (!this->rightOpen) {
    this->right.extend();
    this->rightOpen = true;
  }
}

void Wings::close() {
  this->closeLeft();
  this->closeRight();
}

void Wings::closeLeft() {
  if (this->leftOpen) {
    this->left.contract();
    this->leftOpen = false;
  }
}

void Wings::closeRight() {
  if (this->rightOpen) {
    this->right.contract();
    this->rightOpen = false;
  }
}

void Wings::updateTargeting(control::input::Controller *controller) {
  if (controller->r1Pressed() == 1) {
    if (this->leftOpen) {
      this->closeLeft();
    } else {
      this->openLeft();
    }
  }
  if (controller->r2Pressed() == 1) {
    if (this->rightOpen) {
      this->closeRight();
    } else {
      this->openRight();
    }
  }
}
} // namespace robot
