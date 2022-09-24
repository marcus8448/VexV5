#include <cstring>
#include <fstream>

#include "error.hpp"
#include "logger.hpp"
#include "filesystem.hpp"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "recording.hpp"

RecordingController::RecordingController(pros::Controller controller, const char *filename) : controller(controller) {
  while (!pros::usd::is_installed()) {
    this->controller.set_text(2, 0, "Missing microSD!");
    pros::delay(250);
  }
  this->controller.clear_line(2);

  logger::push_section("Generate ofstream");
  auto fullPath = std::string("/usd/").append(filename).append(".v5r").c_str();
  if (file_exists(fullPath)) {
    bool moved = false;
    for (char i = 0; i < 127; i++) {
      auto name = std::string("/usd/").append(filename).append("_").append(std::to_string(i)).append(
          ".v5r").c_str();
      if (!file_exists(name)) {
        std::ofstream out(name, std::ios::out | std::ios::binary | std::ios::trunc);
        std::ifstream in(fullPath, std::ios::in | std::ios::binary);
        out << std::string((std::istreambuf_iterator<char>(in)),
                           std::istreambuf_iterator<char>()); // apparently rename/copy is not supported
        out.close();
        in.close();
        moved = true;
        printf("Old recording moved to '%s'", name);
        break;
      }
    }
    if (!moved) {
      this->controller.set_text(2, 0, "Too many files!");
      throw;
    }
  }
  controller.clear_line(2);
  this->outf = std::ofstream(filename, std::ios::out | std::ios::binary | std::ios::trunc);
  logger::pop_section();
  this->outf << '0';
}

uint16_t RecordingController::a_pressed() {
  return this->a;
}

uint16_t RecordingController::b_pressed() {
  return this->b;
}

uint16_t RecordingController::x_pressed() {
  return this->x;
}

uint16_t RecordingController::y_pressed() {
  return this->y;
}

uint16_t RecordingController::up_pressed() {
  return this->up;
}

uint16_t RecordingController::down_pressed() {
  return this->down;
}

uint16_t RecordingController::left_pressed() {
  return this->left;
}

uint16_t RecordingController::right_pressed() {
  return this->right;
}

uint16_t RecordingController::l1_pressed() {
  return this->l1;
}

uint16_t RecordingController::l2_pressed() {
  return this->l2;
}

uint16_t RecordingController::r1_pressed() {
  return this->r1;
}

uint16_t RecordingController::r2_pressed() {
  return this->r2;
}

unsigned char RecordingController::digital_speed() {
  return this->digitalSpeed;
}

void RecordingController::digital_speed(unsigned char speed) {
  this->digitalSpeed = speed;
}

void RecordingController::set_line(unsigned char line, unsigned char col, const char *str) {}

void RecordingController::clear_line(unsigned char line) {}

void RecordingController::rumble(const char *str) {}

double RecordingController::left_stick_x() {
  return this->leftStickX;
}

double RecordingController::left_stick_y() {
  return this->leftStickY;
}

double RecordingController::right_stick_x() {
  return this->rightStickX;
}

double RecordingController::right_stick_y() {
  return this->rightStickY;
}

double RecordingController::prev_left_stick_x() {
  return this->prevLeftStickX;
}

double RecordingController::prev_left_stick_y() {
  return this->prevLeftStickY;
}

double RecordingController::prev_right_stick_x() {
  return this->prevRightStickX;
}

double RecordingController::prev_right_stick_y() {
  return this->prevRightStickY;
}

void RecordingController::update() {
  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))) {
    this->a++;
  } else {
    this->a = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))) {
    this->b++;
  } else {
    this->b = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))) {
    this->x++;
  } else {
    this->x = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y))) {
    this->y++;
  } else {
    this->y = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP))) {
    this->up++;
  } else {
    this->up = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))) {
    this->down++;
  } else {
    this->down = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))) {
    this->left++;
  } else {
    this->left = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))) {
    this->right++;
  } else {
    this->right = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))) {
    this->l1++;
  } else {
    this->l1 = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))) {
    this->l2++;
  } else {
    this->l2 = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))) {
    this->r1++;
  } else {
    this->r1 = 0;
  }

  if (check_error(this->controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))) {
    this->r2++;
  } else {
    this->r2 = 0;
  }

  this->prevLeftStickX = this->leftStickX;
  this->leftStickX = check_error(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X));

  this->prevLeftStickY = this->leftStickY;
  this->leftStickY = check_error(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y));

  this->prevRightStickX = this->rightStickX;
  this->rightStickX = check_error(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));

  this->prevRightStickY = this->rightStickY;
  this->rightStickY = check_error(this->controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y));

  if (!this->outf.is_open())
    return;

  if (this->up_pressed() && this->down_pressed()) {
    while (!pros::usd::is_installed()) {
      controller.set_text(2, 0, "Missing microSD!");
      pros::delay(250);
    }
    this->outf.flush();
    this->outf.close();

    controller.set_text(0, 0, "Recording Stopped");
    return;
  }

  if (this->a_pressed() == 1) {
    this->outf << 'a';
  } else if (this->b_pressed() == 1) {
    this->outf << 'b';
  } else if (this->x_pressed() == 1) {
    this->outf << 'x';
  } else if (this->y_pressed() == 1) {
    this->outf << 'y';
  } else if (this->up_pressed() == 1) {
    this->outf << 'u';
  } else if (this->down_pressed() == 1) {
    this->outf << 'd';
  } else if (this->left_pressed() == 1) {
    this->outf << 'l';
  } else if (this->right_pressed() == 1) {
    this->outf << 'r';
  } else if (this->l1_pressed() == 1) {
    this->outf << '!';
  } else if (this->l2_pressed() == 1) {
    this->outf << '@';
  } else if (this->r1_pressed() == 1) {
    this->outf << '#';
  } else if (this->r2_pressed() == 1) {
    this->outf << '$';
  }
  this->outf << '*';
  double val;
  signed long long position;
  val = this->left_stick_x();
  std::memcpy(&position, &val, sizeof(val));
  this->outf << position << " ";
  val = this->left_stick_y();
  std::memcpy(&position, &val, sizeof(val));
  this->outf << position << " ";
  val = this->right_stick_x();
  std::memcpy(&position, &val, sizeof(val));
  this->outf << position << " ";
  val = this->right_stick_y();
  std::memcpy(&position, &val, sizeof(val));
  this->outf << position;
}

void RecordingController::reset() {
}

void RecordingController::stop() {
  this->outf.flush();
  this->outf.close();
}
