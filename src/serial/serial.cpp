#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "pros/rtos.hpp"
#include "serial/serial.hpp"

const uint16_t SIZE = 4;

#define CONNECT "cnct"
#define RECEIVED "recv"
#define RESPONSE "okay"
#define DISCONNECT "gbye"

namespace serial {
static std::vector<SerialPlugin *> PLUGINS;
static bool initialized = false;

enum State {
  NOT_CONNECTED,
  AWAITING_RESPONSE,
  ESTABLISHED
};

State state = NOT_CONNECTED;
uint32_t lastTime = 0;

void timeout_hack(void *params) {
  auto task = static_cast<pros::Task>(params);
  const uint32_t TIMEOUT_LENGTH = 5000;
  while (true) {
    if (state == NOT_CONNECTED) {
      pros::delay(TIMEOUT_LENGTH);
    } else {
      if (pros::millis() - lastTime > TIMEOUT_LENGTH) {
        state = NOT_CONNECTED;
        task.remove(); // kill the task.
        initialize();
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
      if (strcmp(CONNECT, buf) == 0) {
        outputBuf->sputn(RECEIVED, SIZE);
        state = AWAITING_RESPONSE;
      } else {
        if (inputBuf->in_avail() > 0) { // delete all other chars
          inputBuf->pubseekoff(0, std::ios_base::end);
        }
      }
      break;
    case AWAITING_RESPONSE:
      if (strcmp(RESPONSE, buf) == 0) {
        lastTime = pros::millis();
        outputBuf->sputn(RECEIVED, SIZE);
        state = ESTABLISHED;
        for (SerialPlugin *plugin : PLUGINS) {
          plugin->initialize(outputBuf, inputBuf);
        }
      }
      break;
    case ESTABLISHED:
      if (strcmp(DISCONNECT, buf) == 0) {
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

void initialize() {
  pros::Task(debug_input_task, nullptr, "Debug Input Task");
  initialized = true;
}
}
