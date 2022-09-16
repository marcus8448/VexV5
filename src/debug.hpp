#ifndef VEXV5_DEBUG_TOOLS_HPP
#define VEXV5_DEBUG_TOOLS_HPP

#include <streambuf>

class SerialPlugin {
public:
    virtual void clear_state() = 0;
    /**
    * Called when the robot successfully connects to the client computer.
    */
    virtual void initialize(std::streambuf* outputBuf, std::streambuf* inputBuf) = 0;
    virtual bool handle(char type[4]) = 0;
    /**
    * Called when the robot is gracefully disconnected from the computer.
    * Not guarenteed to be called.
    */
    virtual void disconnected() = 0;
};

void add_plugin(SerialPlugin* plugin);

void create_debug_task();

#endif//VEXV5_DEBUG_TOOLS_HPP
