#include "debug.hpp"
#include "pros/rtos.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const uint16_t SIZE = 4;

#define CONNECT "cnct"
#define RECEIVED "recv"
#define RESPONSE "okay"
#define SPECIAL "rqst"
#define DISCONNECT "gbye"

static std::vector<SerialPlugin *> PLUGINS;

enum State {
  NOT_CONNECTED,
  AWAITING_RESPONSE,
  ESTABLISHED
};

State state = NOT_CONNECTED;
long long lastTime = -1;

void timeout_hack(void *params) {
  auto task = static_cast<pros::Task>(params);
  const long long TIMEOUT_LENGTH = 5000;
  while (true) {
    if (state == NOT_CONNECTED) {
      pros::delay(TIMEOUT_LENGTH);
    } else {
      if (pros::millis() - lastTime > TIMEOUT_LENGTH) {
        state = NOT_CONNECTED;
        task.remove(); // kill the task.
        create_debug_task();
        break;
      } else {
        pros::delay(pros::millis() - lastTime + 1);
      }
    }
  }
}

/**
 * Handles debug commands.
 */
[[noreturn]] void debug_input_task([[maybe_unused]] void *params) {
  // everything is static as we kill + re-run the task if the connection times out
  static std::ostringstream bufferFromProgram; // logs from the running program.
  static std::istringstream bufferToProgram; // input to be passed to the program.
  static std::streambuf *outputBuf = std::cout.rdbuf(bufferFromProgram.rdbuf()); // send data through the serial port
  static std::streambuf *inputBuf = std::cin.rdbuf(bufferToProgram.rdbuf()); // read data from the serial port
  bufferFromProgram.clear();
  bufferToProgram.clear();
  static char buf[4];
  for (SerialPlugin *plugin : PLUGINS) {
    plugin->clear_state();
  }

  pros::Task(timeout_hack, static_cast<void *>(pros::Task::current()), "Timeout hack");

  while (true) {
    outputBuf->pubsync();
    lastTime = pros::millis();
    inputBuf->sgetn(buf, SIZE);
    switch (state) {
    case NOT_CONNECTED:
      if (strcmp(CONNECT, buf)) {
        outputBuf->sputn(RECEIVED, SIZE);
        state = AWAITING_RESPONSE;
      } else {
        if (inputBuf->in_avail() > 0) { // delete all other chars
          inputBuf->pubseekoff(0, std::ios_base::end);
        }
      }
      break;
    case AWAITING_RESPONSE:
      if (strcmp(RESPONSE, buf)) {
        lastTime = pros::millis();
        outputBuf->sputn(RECEIVED, SIZE);
        state = ESTABLISHED;
        for (SerialPlugin *plugin : PLUGINS) {
          plugin->initialize(outputBuf, inputBuf);
        }
      }
      break;
    case ESTABLISHED:
      if (strcmp(DISCONNECT, buf)) {
        state = NOT_CONNECTED;
        for (SerialPlugin *plugin : PLUGINS) {
          plugin->disconnected();
        }
        if (inputBuf->in_avail() > 0) { // delete all other chars
          inputBuf->pubseekoff(0, std::ios_base::end);
        }
      } else {
        for (SerialPlugin *plugin : PLUGINS) {
          if (plugin->handle(buf)) {
            lastTime = pros::millis();
            break;
          }
        }
      }
      break;
    }
  }
}

void add_plugin(SerialPlugin *plugin) {
  PLUGINS.push_back(plugin);
}

void create_debug_task() {
  pros::Task(debug_input_task, nullptr, "Debug Input Task");
}
