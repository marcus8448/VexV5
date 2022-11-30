#ifndef SERIAL_ROBOT_STATE_HPP
#define SERIAL_ROBOT_STATE_HPP

#include <streambuf>

#include "robot/robot.hpp"
#include "serial/serial.hpp"

namespace serial {
class RobotStatePlugin : public PacketHandler {
private:
  robot::Robot &robot;

public:
  explicit RobotStatePlugin(robot::Robot &robot);

  void initialize() override;
  void handle(SerialConnection *connection, void *buffer, size_t len) override;
};
} // namespace serial
#endif // SERIAL_ROBOT_STATE_HPP
