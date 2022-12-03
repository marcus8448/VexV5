#ifndef SERIAL_REMOTE_BOT_CONNECTION_HPP
#define SERIAL_REMOTE_BOT_CONNECTION_HPP

#include "serial/serial.hpp"

namespace serial {
class RemoteConnectionPlugin : public PacketHandler {

public:
  explicit RemoteConnectionPlugin(uint8_t id);

  void initialize() override;
  void handle(SerialConnection *connection, void *buffer, size_t len) override;
};
} // namespace serial
#endif // SERIAL_REMOTE_BOT_CONNECTION_HPP
