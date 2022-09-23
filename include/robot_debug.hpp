#ifndef VEXV5_ROBOT_DEBUG_HPP
#define VEXV5_ROBOT_DEBUG_HPP

#include "debug.hpp"
#include "robot.hpp"
#include <vector>
#include <streambuf>

class RobotStatePlugin: public SerialPlugin {
public:
    std::streambuf *outputBuf = nullptr; // do not delete these in the destructor, as they are shared between plugins
    Robot* robot = nullptr;

    explicit RobotStatePlugin(Robot* robot);

    void clear_state() override;
    /**
    * Called when the robot successfully connects to the client computer.
    */
    void initialize(std::streambuf* out, std::streambuf* in) override;
    /**
    * Optionally handle I/O for this type of input. Be sure to read only and all of what you need, otherwise everything will be unaligned.
    */
    bool handle(char type[4]) override;
    /**
    * Called when the robot is gracefully disconnected from the computer.
    * Not guarenteed to be called.
    */
    void disconnected() override;
};

class RobotCommandsPlugin: public SerialPlugin {
public:
    std::streambuf *outputBuf = nullptr; // do not delete these in the destructor, as they are shared between plugins
    std::streambuf *inputBuf = nullptr;
    Robot* robot = nullptr;

    explicit RobotCommandsPlugin(Robot* robot);

    void clear_state() override;
    /**
    * Called when the robot successfully connects to the client computer.
    */
    void initialize(std::streambuf* out, std::streambuf* in) override;
    /**
    * Optionally handle I/O for this type of input. Be sure to read only and all of what you need, otherwise everything will be unaligned.
    */
    bool handle(char type[4]) override;
    /**
    * Called when the robot is gracefully disconnected from the computer.
    * Not guarenteed to be called.
    */
    void disconnected() override;
};

#endif//VEXV5_ROBOT_DEBUG_HPP