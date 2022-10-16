#include "serial/robot_command.hpp"
#include <cstring>
#include <vector>

namespace serial {
#define SERIAL_ROBOT_COMMAND "ROBOT_COMMAND"
#define SERIAL_ROBOT_COMMAND_RESPONSE "ROBOT_COMMAND"

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

void RobotCommandsPlugin::register_packets(IdRegistry *registry) {
  registry->register_packet(SERIAL_ROBOT_COMMAND, this);
}
} // namespace serial
