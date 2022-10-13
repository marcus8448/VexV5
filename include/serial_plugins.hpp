#ifndef SERIALLINK_ROBOT_DEBUG_HPP
#define SERIALLINK_ROBOT_DEBUG_HPP

#include <streambuf>

#include "robot/robot.hpp"
#include "serial/serial.hpp"

namespace serial {
class RobotStatePlugin : public SerialPlugin {
private:
  robot::Robot *robot = nullptr;

public:
  explicit RobotStatePlugin(robot::Robot *robot);

  void initialize() override;
  void handle(SerialConnection *connection, void* buffer, size_t len) override;
  void register_packets(IdRegistry *registry) override;
};

class RobotCommandsPlugin : public SerialPlugin {
public:
  robot::Robot *robot = nullptr;

public:
  explicit RobotCommandsPlugin(robot::Robot *robot);

  void initialize() override;
  void handle(SerialConnection *connection, void* buffer, size_t len) override;
  void register_packets(IdRegistry *registry) override;
};
} // namespace serial
#endif // SERIALLINK_ROBOT_DEBUG_HPP
