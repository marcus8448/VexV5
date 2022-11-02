#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "pros/apix.h"
#include "pros/rtos.hpp"
#include "serial/serial.hpp"

#define SERIAL_RESERVED "SERIAL_RESERVED"
#define SERIAL_CONNECT "SERIAL_CONNECT"
#define SERIAL_RECEIVED "SERIAL_RECEIVED"
#define SERIAL_RESPONSE "SERIAL_RESPONSE"
#define SERIAL_DISCONNECT "SERIAL_DISCONNECT"
#define SERIAL_LOGGING "SERIAL_LOGGING"

namespace serial {
static std::map<const uint16_t, SerialPlugin *> *plugins = new std::map<const uint16_t, SerialPlugin *>();
static IdRegistry *registry = new IdRegistry;
static void *buffer;

SerialConnection *create_serial_connection();

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
[[noreturn]] void debug_input_task(void *params) {
  auto *connection = static_cast<SerialConnection *>(params);
  for (const auto &[k, v] : *plugins) {
    v->initialize();
  }

  pros::Task(timeout_hack, static_cast<void *>(pros::Task::current()), "Timeout hack");

  uint16_t cmd;
  auto cmd_ptr = reinterpret_cast<void *>(&cmd);

  while (true) {
    connection->sync_output();
    lastTime = pros::millis();
    connection->read_exact(cmd_ptr, sizeof(uint16_t));
    const char *command = registry->get_name(cmd);
    switch (state) {
    case NOT_CONNECTED:
      if (strcmp(SERIAL_CONNECT, command) == 0) {
        connection->send_exact(registry->get_id(SERIAL_RECEIVED));
        state = AWAITING_RESPONSE;
      } else {
        connection->skip_to_end();
      }
      break;
    case AWAITING_RESPONSE:
      if (strcmp(SERIAL_RESPONSE, command) == 0) {
        lastTime = pros::millis();
        connection->send_exact(registry->get_id(SERIAL_RECEIVED));
        state = ESTABLISHED;
      }
      break;
    case ESTABLISHED:
      if (strcmp(SERIAL_DISCONNECT, command) == 0) {
        state = NOT_CONNECTED;
        connection->skip_to_end();
      } else {
        uint16_t len = connection->read_variable(buffer, 512);
        if (len == 0) {
          break;
        }
        if (plugins->count(cmd)) {
          plugins->at(cmd)->handle(connection, buffer, len);
          lastTime = pros::millis();
        }
      }
      break;
    }
  }
}

void add_plugin(const uint16_t id, SerialPlugin *plugin) {
  plugins->emplace(id, plugin);
  plugin->register_packets(registry);
}

void initialize() {
  if (buffer == nullptr) {
    buffer = malloc(512);
  }
  pros::Task(debug_input_task, create_serial_connection(), "Debug Input Task");
}

SerialConnection *create_serial_connection() {
  static SerialConnection *connection = nullptr;
  pros::c::serctl(SERCTL_DISABLE_COBS, nullptr);
  if (connection == nullptr) {
    connection = new SerialConnection(std::cout.rdbuf(), std::cin.rdbuf(), registry);
  }
  connection->skip_to_end();
  return connection;
}

IdRegistry::IdRegistry()
    : idToName(new std::map<const uint16_t, const char *>()),
      idToPlugin(new std::map<const uint16_t, SerialPlugin *>()),
      nameToId(new std::map<const char *, const uint16_t>()) {
  this->register_internal(SERIAL_RESERVED);
  this->register_internal(SERIAL_CONNECT);
  this->register_internal(SERIAL_RECEIVED);
  this->register_internal(SERIAL_RESPONSE);
  this->register_internal(SERIAL_DISCONNECT);
}

uint16_t IdRegistry::register_packet(const char *name, SerialPlugin *plugin) {
  this->idToName->emplace(this->size, name);
  this->idToPlugin->emplace(this->size, plugin);
  this->nameToId->emplace(name, this->size);

  return this->size++;
}

uint16_t IdRegistry::register_internal(const char *name) {
  this->idToName->emplace(this->size, name);
  this->idToPlugin->emplace(this->size, nullptr);
  this->nameToId->emplace(name, this->size);

  return this->size++;
}

const char *IdRegistry::get_name(const uint16_t id) { return this->idToName->at(id); }

uint16_t IdRegistry::get_id(const char *name) { return this->nameToId->at(name); }

SerialConnection::SerialConnection(std::streambuf *output, std::streambuf *input, IdRegistry *registry)
    : output(output), input(input), registry(registry) {}

uint16_t SerialConnection::read_variable(void *ptr, const uint16_t len) {
  uint16_t length = 0;
  this->input->sgetn(static_cast<char *>(static_cast<void *>(&length)), sizeof(uint16_t));
  if (length >= len) {
    return 0; // todo
  }
  this->input->sgetn(static_cast<char *>(ptr), length);
  *(static_cast<char *>(ptr) + length) = '\0';
  return length;
}

void SerialConnection::read_exact(void *ptr, const uint16_t size) {
  this->input->sgetn(static_cast<char *>(ptr), size);
}

void SerialConnection::read_null_term(char *ptr, const uint16_t read) {
  this->input->sgetn(ptr, read);
  *(ptr + read) = '\0'; // ensure null-terminated
}

void SerialConnection::sync_output() { this->output->pubsync(); }

void SerialConnection::send_suffix() { this->output->sputn("#\n", 3); }

size_t SerialConnection::available() { return this->input->in_avail(); }

void SerialConnection::skip_to_end() {
  if (this->available() > 0) {
    this->input->pubseekoff(0, std::ios_base::end);
  }
}

void SerialConnection::send_header() { this->output->sputn("^#", 2); }

void SerialConnection::send(const char *name, const void *data, const uint16_t len) {
  auto id = this->registry->get_id(name);
  this->send_header();
  this->output->sputn(reinterpret_cast<const char *>(&id), sizeof(uint16_t));
  this->output->sputn(reinterpret_cast<const char *>(&len), sizeof(uint16_t));
  this->output->sputn(static_cast<const char *>(data), len);
  this->send_suffix();
}

void SerialConnection::send_exact(const void *data, const uint16_t len) {
  this->send_header();
  this->output->sputn(reinterpret_cast<const char *>(data), len);
  this->send_suffix();
}

void SerialConnection::send_exact(const uint16_t data) { this->send_exact(&data, sizeof(uint16_t)); }
} // namespace serial
