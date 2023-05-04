#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "debug/logger.hpp"
#include "pros/apix.h"
#include "pros/rtos.hpp"
#include "serial/serial.hpp"

#define SERIAL_RESERVED 0
#define SERIAL_CONNECT 1
#define SERIAL_RECEIVED 2
#define SERIAL_RESPONSE 3
#define SERIAL_DISCONNECT 4

#define BUFFER_SIZE 512
#define TIMEOUT_LENGTH 5000

namespace serial {
static std::map<const uint8_t, PacketHandler *> *packet_handlers = new std::map<const uint8_t, PacketHandler *>;
static void *BUFFER = malloc(BUFFER_SIZE);

enum State { NOT_CONNECTED, AWAITING_RESPONSE, ESTABLISHED };

State state = NOT_CONNECTED;
uint32_t lastTime = 0;

void timeout_hack(void *params) {
  auto task = static_cast<pros::task_t>(params);
  while (true) {
    if (state == NOT_CONNECTED) {
      pros::delay(TIMEOUT_LENGTH);
    } else {
      if (pros::millis() - lastTime > TIMEOUT_LENGTH) {
        state = NOT_CONNECTED;
        pros::c::task_delete(task); // kill the task.
        info("Serial connection timed out.");
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
[[noreturn]] void debug_input_task(void *params) {
  auto *connection = static_cast<SerialConnection *>(params);
  for (const auto &[k, v] : *packet_handlers) {
    v->initialize();
  }

  pros::c::task_create(timeout_hack, static_cast<void *>(pros::c::task_get_current()), TASK_PRIORITY_DEFAULT,
                       TASK_STACK_DEPTH_DEFAULT, "Timeout hack");

  while (true) {
    connection->sync_output();
    pros::delay(30);
    lastTime = pros::millis();
    PacketData data = connection->read_packet(BUFFER, BUFFER_SIZE);
    switch (state) {
    case NOT_CONNECTED:
      if (SERIAL_CONNECT == data.id) {
        connection->send_packet(SERIAL_RECEIVED, nullptr, 0);
        state = AWAITING_RESPONSE;
      } else {
        connection->skip_to_end();
      }
      break;
    case AWAITING_RESPONSE:
      if (SERIAL_RESPONSE == data.id) {
        lastTime = pros::millis();
        connection->send_packet(SERIAL_RECEIVED, nullptr, 0);
        state = ESTABLISHED;
      }
      break;
    case ESTABLISHED:
      if (SERIAL_DISCONNECT == data.id || data.data == nullptr) {
        state = NOT_CONNECTED;
        connection->skip_to_end();
      } else {
        packet_handlers->at(data.id)->handle(connection, data.data, data.len);
        lastTime = pros::millis();
      }
      break;
    }
  }
}

void register_packet_handler(const uint8_t id, PacketHandler *handler) {
  if (id >= 85 || id < 8) {
    error("Ignoring packet handler with out of bounds id %i", id);
    return;
  }
  packet_handlers->emplace(id, handler);
}

void initialize() {
  pros::c::task_create(debug_input_task, static_cast<void *>(&SerialConnection::get_instance()), TASK_PRIORITY_DEFAULT,
                       TASK_STACK_DEPTH_DEFAULT, "Debug input");
}
} // namespace serial
