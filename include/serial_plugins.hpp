#ifndef SERIALLINK_ROBOT_DEBUG_HPP
#define SERIALLINK_ROBOT_DEBUG_HPP

#include <vector>
#include <streambuf>

#include "serial/serial.hpp"
#include "robot/robot.hpp"

namespace serial {
class RobotStatePlugin : public SerialPlugin {
public:
  std::streambuf *outputBuf = nullptr; // do not delete these in the destructor, as they are shared between plugins
  robot::Robot *robot = nullptr;

  explicit RobotStatePlugin(robot::Robot *robot);

  void clear_state() override;
  /**
  * Called when the robot successfully connects to the client computer.
  */
  void initialize(std::streambuf *out, std::streambuf *in) override;
  /**
  * Optionally handle I/O for this type of input. Be sure to read only and all of what you need, otherwise everything will be unaligned.
  */
  bool handle(const char type[4]) override;
  /**
  * Called when the robot is gracefully disconnected from the computer.
  * Not guaranteed to be called.
  */
  void disconnected() override;
};

class RobotCommandsPlugin : public SerialPlugin {
public:
  std::streambuf *outputBuf = nullptr; // do not delete these in the destructor, as they are shared between plugins
  std::streambuf *inputBuf = nullptr;
  robot::Robot *robot = nullptr;

  explicit RobotCommandsPlugin(robot::Robot *robot);

  void clear_state() override;
  /**
  * Called when the robot successfully connects to the client computer.
  */
  void initialize(std::streambuf *out, std::streambuf *in) override;
  /**
  * Optionally handle I/O for this type of input. Be sure to read only and all of what you need, otherwise everything will be unaligned.
  */
  bool handle(const char type[4]) override;
  /**
  * Called when the robot is gracefully disconnected from the computer.
  * Not guarenteed to be called.
  */
  void disconnected() override;
};
}
#endif//SERIALLINK_ROBOT_DEBUG_HPP
