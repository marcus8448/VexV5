#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "pros/apix.h"
#include "pros/rtos.hpp"
#include "serial/serial.hpp"

const size_t SIZE = sizeof(uint16_t);

#define RESERVED 0
#define CONNECT 1
#define RECEIVED 2
#define RESPONSE 3
#define DISCONNECT 4
#define LOGGING 5

namespace serial {
static std::map<const uint16_t, SerialPlugin *> PLUGINS = {};
static bool initialized = false;
static void* buffer;

enum State { NOT_CONNECTED, AWAITING_RESPONSE, ESTABLISHED };

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
  static std::ostringstream bufferFromProgram;                                   // logs from the running program.
  static std::istringstream bufferToProgram;                                     // input to be passed to the program.
  static std::streambuf *outputBuf = std::cout.rdbuf(bufferFromProgram.rdbuf()); // send data through the serial port
  static std::streambuf *inputBuf = std::cin.rdbuf(bufferToProgram.rdbuf());     // read data from the serial port
  SerialConnection connection = SerialConnection(outputBuf, inputBuf);
  bufferFromProgram.clear();
  bufferToProgram.clear();
  for (const auto &[k, v] : PLUGINS) {
    v->initialize();
  }

  pros::Task(timeout_hack, static_cast<void *>(pros::Task::current()), "Timeout hack");

  uint16_t cmd;
  auto cmdc = reinterpret_cast<char *>(&cmd);

  while (true) {
    outputBuf->pubsync();
    lastTime = pros::millis();
    inputBuf->sgetn(cmdc, SIZE);
    switch (state) {
    case NOT_CONNECTED:
      if (cmd == CONNECT) {
        outputBuf->sputn(reinterpret_cast<const char *>(RECEIVED), SIZE);
        state = AWAITING_RESPONSE;
      } else {
        if (inputBuf->in_avail() > 0) { // delete all other chars
          inputBuf->pubseekoff(0, std::ios_base::end);
        }
      }
      break;
    case AWAITING_RESPONSE:
      if (cmd == RESPONSE) {
        lastTime = pros::millis();
        outputBuf->sputn(reinterpret_cast<const char *>(RECEIVED), SIZE);
        state = ESTABLISHED;
      }
      break;
    case ESTABLISHED:
      if (cmd == DISCONNECT) {
        state = NOT_CONNECTED;
        if (inputBuf->in_avail() > 0) { // delete all other chars
          inputBuf->pubseekoff(0, std::ios_base::end);
        }
      } else {
        uint16_t len = connection.read(buffer, 512);
        if (len == 0) {
          break;
        }
        if (PLUGINS.count(cmd)) {
          PLUGINS.at(cmd)->handle(&connection, buffer, len);
          lastTime = pros::millis();
        }
      }
      break;
    }
  }
}

void add_plugin(const int16_t id, SerialPlugin *plugin) {
  PLUGINS[id] = plugin;
}

void initialize() {
  if (buffer == nullptr) {
    buffer = malloc(512);
  }
  pros::Task(debug_input_task, nullptr, "Debug Input Task");
}

SerialConnection::SerialConnection(std::streambuf *output, std::streambuf *input) : output(output), input(input) {}

uint16_t SerialConnection::read(void *ptr, uint16_t len) {
  uint16_t length = 0;
  this->input->sgetn(reinterpret_cast<char *>(&length), sizeof(uint16_t));
  if (length > len) {
    return 0; //todo
  }
  this->input->sgetn(static_cast<char *>(ptr), length);
  return length;
}

void SerialConnection::send(const char *id, void *data, uint16_t len) {
  this->output->sputn(id, 4);
  this->output->sputn(reinterpret_cast<const char *>(&len), sizeof(uint16_t));
  this->output->sputn(static_cast<const char *>(data), len);
}
} // namespace serial
