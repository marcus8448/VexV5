#include "serial_plugins.hpp"
#include <cstring>
#include <vector>

namespace serial {
#define SERIAL_RSTATE "ROBOT_STATE"
#define SERIAL_RCOMMAND "ROBOT_STATE"
#define SERIAL_RCOMMAND_RESPONSE "ROBOT_STATE"


typedef double floating;

RobotStatePlugin::RobotStatePlugin(robot::Robot *robot) : robot(robot) {}

void RobotStatePlugin::initialize() {}

void serialize_motor(uint8_t *buffer, pros::Motor *motor) {
  floating src;
  src = motor->get_target_position();
  std::memcpy(&buffer[sizeof(floating) * 0], &src, sizeof(floating));
  src = motor->get_actual_velocity();
  std::memcpy(&buffer[sizeof(floating) * 1], &src, sizeof(floating));
  src = motor->get_position();
  std::memcpy(&buffer[sizeof(floating) * 2], &src, sizeof(floating));
  src = motor->get_efficiency();
  std::memcpy(&buffer[sizeof(floating) * 3], &src, sizeof(floating));
  src = motor->get_power();
  std::memcpy(&buffer[sizeof(floating) * 4], &src, sizeof(floating));
  src = motor->get_temperature();
  std::memcpy(&buffer[sizeof(floating) * 5], &src, sizeof(floating));
  src = motor->get_torque();
  std::memcpy(&buffer[sizeof(floating) * 6], &src, sizeof(floating));
  uint32_t isrc;
  isrc = motor->get_target_velocity();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 0], &isrc, sizeof(int));
  isrc = motor->get_current_draw();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 1], &isrc, sizeof(int));
  isrc = motor->get_voltage();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 2], &isrc, sizeof(int));
  isrc = motor->get_current_limit();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 3], &isrc, sizeof(int));
  isrc = motor->get_voltage_limit();
  std::memcpy(&buffer[sizeof(floating) * 7 + sizeof(int) * 4], &isrc, sizeof(int));
}

void RobotStatePlugin::handle(serial::SerialConnection *connection, void *buffer, size_t len) {
  const uint32_t CONTROLLER_SIZE = 3 + (sizeof(float) * 4);
  const uint32_t MOTOR_SIZE = (sizeof(floating) * 7) + (sizeof(int) * 5);
  const uint32_t SIZE = CONTROLLER_SIZE + (MOTOR_SIZE * 4);
  static uint8_t buf[SIZE] = {0};

  if (this->robot->controller->a_pressed())
    buf[0] |= 0b00000001;
  if (this->robot->controller->b_pressed())
    buf[0] |= 0b00000010;
  if (this->robot->controller->x_pressed())
    buf[0] |= 0b00000100;
  if (this->robot->controller->y_pressed())
    buf[0] |= 0b00001000;
  if (this->robot->controller->up_pressed())
    buf[0] |= 0b00010000;
  if (this->robot->controller->down_pressed())
    buf[0] |= 0b00100000;
  if (this->robot->controller->left_pressed())
    buf[0] |= 0b01000000;
  if (this->robot->controller->right_pressed())
    buf[0] |= 0b10000000;
  if (this->robot->controller->l1_pressed())
    buf[1] |= 0b00000001;
  if (this->robot->controller->l2_pressed())
    buf[1] |= 0b00000010;
  if (this->robot->controller->r1_pressed())
    buf[1] |= 0b00000100;
  if (this->robot->controller->r2_pressed())
    buf[1] |= 0b00001000;
  buf[2] = this->robot->controller->digital_speed();
  float src;
  src = (float) this->robot->controller->left_stick_x();
  std::memcpy(&buf[3 + sizeof(float) * 0], &src, sizeof(float));
  src = (float) this->robot->controller->left_stick_y();
  std::memcpy(&buf[3 + sizeof(float) * 1], &src, sizeof(float));
  src = (float) this->robot->controller->right_stick_x();
  std::memcpy(&buf[3 + sizeof(float) * 2], &src, sizeof(float));
  src = (float) this->robot->controller->right_stick_y();
  std::memcpy(&buf[3 + sizeof(float) * 3], &src, sizeof(float));

  serialize_motor(&buf[CONTROLLER_SIZE + MOTOR_SIZE * 0], this->robot->drivetrain->rightFront);
  serialize_motor(&buf[CONTROLLER_SIZE + MOTOR_SIZE * 1], this->robot->drivetrain->leftFront);
  serialize_motor(&buf[CONTROLLER_SIZE + MOTOR_SIZE * 2], this->robot->drivetrain->rightBack);
  serialize_motor(&buf[CONTROLLER_SIZE + MOTOR_SIZE * 3], this->robot->drivetrain->leftBack);
  connection->send(SERIAL_RSTATE, &buf, SIZE);
}

RobotCommandsPlugin::RobotCommandsPlugin(robot::Robot *robot) : robot(robot) {}

void RobotCommandsPlugin::initialize() {}

void RobotCommandsPlugin::handle(serial::SerialConnection *connection, void *buffer, size_t len) {
  char *command = static_cast<char *>(buffer);
  if (strlen(command) > len) {
    return;
  }

  for (size_t i = 0; command[i]; i++) {
    command[i] = tolower(command[i]);
  }

  std::vector<std::string> vec;
  do {
    command = strtok(command, " ");
    vec.emplace_back(command);
  } while (command != nullptr);

  size_t points = vec.size();

  if (points > 0) {
    if (vec[0] == "set") {
      if (points > 3) {
        if (vec[1] == "drivetrain") {
          if (points > 4) {
            if (vec[2] == "right_front") {
              if (vec[3] == "target_velocity") {

              } else if (vec[3] == "target_voltage") {

              } else if (vec[3] == "target_position") {

              } else if (vec[3] == "relative_target_position") {
              }
            } else if (vec[2] == "right_back") {

            } else if (vec[2] == "left_front") {

            } else if (vec[2] == "left_back") {
            }
          }
        }
      }
    } else if (vec[0] == "get") {
      if (points > 1) {

      }
    }
  }
}
}
