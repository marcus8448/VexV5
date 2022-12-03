#include "serial/robot_command.hpp"
#include <cstring>
#include <vector>

namespace serial {
RobotCommandsPlugin::RobotCommandsPlugin(uint8_t id, robot::Robot &robot) : PacketHandler(id), robot(robot) {}

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
  command = strtok(command, " ");
  while (command != nullptr) {
    vec.emplace_back(command);
    command = strtok(command, " ");
  }

  size_t points = vec.size();

  if (points > 0) {
    if (vec[0] == "drivetrain") {
      if (vec[1] == "right_front") {
        if (vec[3] == "target_velocity") {

        } else if (vec[3] == "target_voltage") {

        } else if (vec[3] == "target_position") {

        } else if (vec[3] == "relative_target_position") {
        }
      } else if (vec[1] == "right_back") {

      } else if (vec[1] == "left_front") {

      } else if (vec[1] == "left_back") {
      }
    }
  }
}
} // namespace serial
