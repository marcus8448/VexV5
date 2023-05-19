#ifndef SERIAL_ROBOT_COMMAND_HPP
#define SERIAL_ROBOT_COMMAND_HPP

#include "robot/robot.hpp"
#include "serial.hpp"

namespace serial {
class RobotCommandsPlugin : public PacketHandler {
public:
  robot::Robot &robot;

public:
  explicit RobotCommandsPlugin(uint8_t id, robot::Robot &robot);

  void initialize() override;
  void handle(SerialConnection *connection, void *buffer, size_t len) override;
};
} // namespace serial
#endif // SERIAL_ROBOT_COMMAND_HPP
