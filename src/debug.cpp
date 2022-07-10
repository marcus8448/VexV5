#include "debug.hpp"
#include "pros/rtos.hpp"
#include <iostream>
#include <sstream>
#include <string>

const int SIZE = 4;

const char* CONNECT = "cnct";
const char* RECIEVED = "recv";
const char* RESPONSE = "okay";
const char* SPECIAL = "rqst";
const char* DISCONNECT = "gbye";

enum State {
    NOT_CONNECTED,
    AWAITING_RESPONSE,
    ESTABLISHED
};

State state = NOT_CONNECTED;
long long lastTime = -1;

/**
 * Handles debug commands.
 */
void debug_input_task(void* _) {
    // everything is static as we kill + re-run the task if the connection times out
    static std::ostringstream log_buf; // logs from the running program.
    static std::istringstream in_passthrough_buf; // input to be passed to the program.
    static std::streambuf* raw_out = std::cout.rdbuf(log_buf.rdbuf()); // send data through the serial port
    static std::streambuf* raw_in = std::cin.rdbuf(in_passthrough_buf.rdbuf()); // read data from the serial port

    static char buf[4];
    for (SerialPlugin* plugin : PLUGINS) {
        plugin->clear_state();
    }

    while (true) {
        raw_out->pubsync();
        lastTime = pros::millis();
        raw_in->sgetn(buf, SIZE);
        switch (state) {
            case NOT_CONNECTED:
                if (buf == CONNECT) {
                    raw_out->sputn(RECIEVED, SIZE);
                    state = AWAITING_RESPONSE;
                }
                break;
            case AWAITING_RESPONSE:
                if (buf == RESPONSE) {
                    raw_out->sputn(RECIEVED, SIZE);
                    state = ESTABLISHED;
                    for (SerialPlugin* plugin : PLUGINS) {
                        plugin->initialize(raw_in, raw_out);
                    }
                }
                break;
            case ESTABLISHED:
                if (buf == DISCONNECT) {
                    state = NOT_CONNECTED;
                    for (SerialPlugin* plugin : PLUGINS) {
                        plugin->disconnected();
                    }
                } else {
                    lastTime = pros::millis();
                    for (SerialPlugin* plugin : PLUGINS) {
                        plugin->handle(buf);
                    }
                }
            break;
        }
    }
}

void create_debug_task() {
    pros::Task(debug_input_task, nullptr, "Debug Input Task");
}

void timeout_hack(void* param) {
    auto task = static_cast<pros::Task*>(param);
    const long long TIMEOUT_LENGTH = 5000;
    while (true) {
        if (state == NOT_CONNECTED) {
            pros::delay(TIMEOUT_LENGTH);
        } else {
            if (pros::millis() - lastTime > TIMEOUT_LENGTH) {
                state = NOT_CONNECTED;
                task->remove(); // kill the task.
                create_debug_task();
                break;
            } else {
                pros::delay(pros::millis() - lastTime + 1);
            }
        }
    }
}
