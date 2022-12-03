#ifndef SERIAL_DEBUG_HPP
#define SERIAL_DEBUG_HPP

#include <map>
#include <streambuf>
#include "serial/serial_connection.hpp"

namespace serial {
class PacketHandler {
protected:
  const uint8_t id;

public:
  explicit PacketHandler(uint8_t id): id(id) {};

  virtual void initialize() = 0;
  virtual void handle(SerialConnection *connection, void *buffer, size_t len) = 0;
};

void register_packet_handler(uint8_t id, PacketHandler *handler);

void initialize();
} // namespace serial
#endif // SERIAL_DEBUG_HPP
