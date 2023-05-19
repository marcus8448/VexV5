#include "control/input/autonomous_outline.hpp"
#include "debug/error.hpp"
#include "debug/logger.hpp"
#include "units.hpp"

#define controller_digital(button) print_error(pros::c::controller_get_digital(this->controller_id, button))
#define AUTOSPEED 35.0

namespace control::input {
AutonomousOutlineController::AutonomousOutlineController(robot::Robot &robot, pros::controller_id_e_t controller_id)
    : controller_id(controller_id), robot(robot), output(fs::open_indexed("autonomous", std::ios::out)) {
  this->output << "// AUTON START\n";
}

[[nodiscard]] uint16_t AutonomousOutlineController::aPressed() const { return this->a; }

[[nodiscard]] uint16_t AutonomousOutlineController::bPressed() const { return this->b; }

[[nodiscard]] uint16_t AutonomousOutlineController::xPressed() const {
  return this->x >= 2 && this->flywheelRuntime != -1;
}

[[nodiscard]] uint16_t AutonomousOutlineController::yPressed() const { return this->y; }

[[nodiscard]] uint16_t AutonomousOutlineController::upPressed() const { return false; }

[[nodiscard]] uint16_t AutonomousOutlineController::downPressed() const { return false; }

[[nodiscard]] uint16_t AutonomousOutlineController::leftPressed() const { return false; }

[[nodiscard]] uint16_t AutonomousOutlineController::rightPressed() const { return false; }

[[nodiscard]] uint16_t AutonomousOutlineController::l1Pressed() const { return this->l1; }

[[nodiscard]] uint16_t AutonomousOutlineController::l2Pressed() const { return this->l2; }

[[nodiscard]] uint16_t AutonomousOutlineController::r1Pressed() const { return this->r1; }

[[nodiscard]] uint16_t AutonomousOutlineController::r2Pressed() const { return this->r2; }

[[nodiscard]] double AutonomousOutlineController::leftStickX() const { return 0; }

[[nodiscard]] double AutonomousOutlineController::leftStickY() const {
  if (!this->targetActive)
    return 0;
  switch (this->drivetrainTarget) {
  case FORWARDS:
  case RIGHT:
    return AUTOSPEED;
  case BACKWARDS:
  case LEFT:
    return -AUTOSPEED;
  }
  return 0;
}

[[nodiscard]] double AutonomousOutlineController::rightStickX() const { return 0; }

[[nodiscard]] double AutonomousOutlineController::rightStickY() const {
  if (!this->targetActive)
    return 0;
  switch (this->drivetrainTarget) {
  case FORWARDS:
  case LEFT:
    return AUTOSPEED;
  case BACKWARDS:
  case RIGHT:
    return -AUTOSPEED;
  }
  return 0;
}

[[nodiscard]] int16_t AutonomousOutlineController::speedSetting() const { return this->flywheelSpeed; }

void AutonomousOutlineController::setSpeedSetting(int16_t speed) { this->flywheelSpeed = speed; }

void AutonomousOutlineController::setLine(uint8_t line, uint8_t col, const char *str) {
  print_error(pros::c::controller_set_text(this->controller_id, line, col, str));
}

void AutonomousOutlineController::clearLine(uint8_t line) {
  print_error(pros::c::controller_clear_line(this->controller_id, line));
}

void AutonomousOutlineController::rumble(const char *str) {
  clear_error();
  pros::c::controller_rumble(this->controller_id, str);
  if (get_error() == EAGAIN) {
    debug("Failed to send rumble. Trying again soon.");
    this->enqueued_rumble = str;
  }
}

void AutonomousOutlineController::update() {
  if (this->x > 125) {
    info("writing out recording!");
    pros::c::controller_rumble(this->controller_id, "-");
    this->write_drivetrain_update();
    this->output << "// AUTONOMOUS END\n";
    this->output.close(); // pros/vex only writes to the microSD on close.
  }

  this->ticks++;
  if (controller_digital(pros::E_CONTROLLER_DIGITAL_A)) {
    this->a++;
  } else {
    this->a = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_B)) {
    this->b++;
  } else {
    this->b = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_X)) {
    this->x++;
  } else {
    this->x = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
    this->y++;
  } else {
    this->y = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
    this->up++;
  } else {
    this->up = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
    this->down++;
  } else {
    this->down = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
    this->left++;
  } else {
    this->left = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
    this->right++;
  } else {
    this->right = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
    this->l1++;
  } else {
    this->l1 = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
    this->l2++;
  } else {
    this->l2 = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
    this->r1++;
  } else {
    this->r1 = 0;
  }

  if (controller_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
    this->r2++;
  } else {
    this->r2 = 0;
  }

  bool changed = false;

  if (this->up) {
    if (this->drivetrainTarget != FORWARDS || !this->targetActive) {
      if (this->drivetrainTarget != FORWARDS)
        changed = this->write_drivetrain_update();
      this->drivetrainTarget = FORWARDS;
      this->targetDirty = true;
      this->targetActive = true;
    }
  } else if (this->down) {
    if (this->drivetrainTarget != BACKWARDS || !this->targetActive) {
      if (this->drivetrainTarget != BACKWARDS)
        changed = this->write_drivetrain_update();
      this->drivetrainTarget = BACKWARDS;
      this->targetDirty = true;
      this->targetActive = true;
    }
  } else if (this->left) {
    if (this->drivetrainTarget != LEFT || !this->targetActive) {
      if (this->drivetrainTarget != LEFT)
        changed = this->write_drivetrain_update();
      this->drivetrainTarget = LEFT;
      this->targetDirty = true;
      this->targetActive = true;
    }
  } else if (this->right) {
    if (this->drivetrainTarget != RIGHT || !this->targetActive) {
      if (this->drivetrainTarget != RIGHT)
        changed = this->write_drivetrain_update();
      this->drivetrainTarget = RIGHT;
      this->targetDirty = true;
      this->targetActive = true;
    }
  } else {
    this->targetActive = false;
  }

  if (this->x % 2 == 1 && this->flywheelRuntime == -1) {
    this->setSpeedSetting(static_cast<int16_t>(std::min(this->speedSetting() + 100, 12000)));
  } else if (this->yPressed() % 2 == 1) {
    this->setSpeedSetting(static_cast<int16_t>(std::max(this->speedSetting() - 100, 4000)));
  }

  if (this->r1 == 1) {
    this->write_drivetrain_update();
    this->flywheelRuntime = 0;
    this->output << "robot.flywheel->engage(" << this->flywheelSpeed << ");\n";
    changed = true;
  }

  if (this->flywheelRuntime != -1) {
    this->flywheelRuntime++;
    if (this->xPressed() > 2) {
      this->write_drivetrain_update();
      this->flywheelRuntime = -1;
      this->output << "robot.flywheel->disengage();\n";
      changed = true;
    }
  }

  if (this->l1Pressed() && this->l2Pressed()) {
    this->write_drivetrain_update();
    this->output << "robot.intake->reverse();\n";
    changed = true;
  } else if (this->l1Pressed() == 3) {
    this->write_drivetrain_update();
    this->output << "robot.intake->engage();\n";
    changed = true;
  } else if (this->l2Pressed() == 3) {
    this->write_drivetrain_update();
    this->output << "robot.intake->disengage();\n";
    changed = true;
  }

  if (this->r2Pressed() == 1) {
    if (this->flywheelRuntime != -1) {
      this->write_drivetrain_update();
      this->output << "robot.indexer->cycle();\n";
      changed = true;
    }
  }

  if (changed) {
    this->output.flush();
  }

  if (this->a == 1)
    debug("A pressed");
  if (this->b == 1)
    debug("B pressed");
  if (this->x == 1)
    debug("X pressed");
  if (this->y == 1)
    debug("Y pressed");
  if (this->up == 1)
    debug("Up pressed");
  if (this->down == 1)
    debug("Down pressed");
  if (this->left == 1)
    debug("Left pressed");
  if (this->right == 1)
    debug("Right pressed");
  if (this->r1 == 1)
    debug("R1 pressed");
  if (this->r2 == 1)
    debug("R2 pressed");
  if (this->l1 == 1)
    debug("L1 pressed");
  if (this->l2 == 1)
    debug("L2 pressed");

  if (enqueued_rumble != nullptr && this->ticks % 10 == 0) {
    clear_error();
    this->rumble(this->enqueued_rumble);
    if (get_error() != EAGAIN) {
      this->enqueued_rumble = nullptr;
    }
  }

  static bool init = false;
  if (this->ticks % 10 == 0 || !init) {
    init = true;
    this->setLine(0, 0,
                  logger::string_format("Flywheel: %i  ", static_cast<int32_t>(this->speedSetting()))
                      .c_str()); // append ' ' to clear out buffer
  }
}

bool AutonomousOutlineController::write_drivetrain_update() {
  if (!this->targetDirty) {
    this->robot.drivetrain.tare();
    return false;
  }
  this->targetDirty = false;
  switch (this->drivetrainTarget) {
  case FORWARDS: {
    double dist = units::encoderToInch(
        (this->robot.drivetrain.leftFront.getPosition() + this->robot.drivetrain.leftBack.getPosition() +
         this->robot.drivetrain.rightFront.getPosition() + this->robot.drivetrain.rightBack.getPosition()) /
        4.0);
    //    debug("Forwards %f", (this->robot.drivetrain.leftFront.get_position() +
    //    this->robot.drivetrain.leftBack.get_position() +
    //                          this->robot.drivetrain.rightFront.get_position() +
    //                          this->robot.drivetrain.rightBack.get_position()) /
    //                             4.0);
    debug("Forwards %f", dist);
    this->robot.drivetrain.tare();
    this->output << "robot.drivetrain.forwards(" << dist << ");\n";
    return true;
  }
  case BACKWARDS: {
    double dist = units::encoderToInch(
        (this->robot.drivetrain.leftFront.getPosition() + this->robot.drivetrain.leftBack.getPosition() +
         this->robot.drivetrain.rightFront.getPosition() + this->robot.drivetrain.rightBack.getPosition()) /
        4.0);
    //    debug("Backwards %f", (this->robot.drivetrain.leftFront.get_position() +
    //    this->robot.drivetrain.leftBack.get_position() +
    //                           this->robot.drivetrain.rightFront.get_position() +
    //                           this->robot.drivetrain.rightBack.get_position()) /
    //                              -4.0);
    debug("Backwards %f", dist);
    this->robot.drivetrain.tare();
    this->output << "robot.drivetrain.backwards(" << -dist << ");\n";
    return true;
  }
  case LEFT: {
    double dist_left = units::encoderToDegrees(
        (this->robot.drivetrain.leftFront.getPosition() + this->robot.drivetrain.leftBack.getPosition()) / 2.0);
    double dist_right = units::encoderToDegrees(
        (this->robot.drivetrain.rightFront.getPosition() + this->robot.drivetrain.rightBack.getPosition()) / 2.0);
    //    debug("Left %f %f", (this->robot.drivetrain.rightFront.get_position() +
    //    this->robot.drivetrain.rightBack.get_position()) / 2.0, (this->robot.drivetrain.leftFront.get_position() +
    //    this->robot.drivetrain.leftBack.get_position()) / -2.0);
    this->robot.drivetrain.tare();
    double turn = (dist_right - dist_left) / 2.0;
    debug("Left %f", turn);
    this->output << "robot.drivetrain.turnLeft(" << turn << ");\n";
    return true;
  }
  case RIGHT: {
    double dist_left = units::encoderToDegrees(
        (this->robot.drivetrain.leftFront.getPosition() + this->robot.drivetrain.leftBack.getPosition()) / 2.0);
    double dist_right = units::encoderToDegrees(
        (this->robot.drivetrain.rightFront.getPosition() + this->robot.drivetrain.rightBack.getPosition()) / 2.0);
    //    debug("Right %f %f", (this->robot.drivetrain.leftFront.get_position() +
    //    this->robot.drivetrain.leftBack.get_position()) / 2.0, (this->robot.drivetrain.rightFront.get_position() +
    //    this->robot.drivetrain.rightBack.get_position()) / -2.0);
    this->robot.drivetrain.tare();
    double turn = (dist_left - dist_right) / 2.0;
    debug("Right %f", turn);
    this->output << "robot.drivetrain.turnRight(" << turn << ");\n";
    return true;
  }
  }
  return false;
}
} // namespace control::input
