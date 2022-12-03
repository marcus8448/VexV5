#include "serial/remote_bot_connection.hpp"

namespace serial {
RemoteConnectionPlugin::RemoteConnectionPlugin(uint8_t id) : PacketHandler(id) {}

void RemoteConnectionPlugin::initialize() {}

void RemoteConnectionPlugin::handle(serial::SerialConnection *connection, void *buffer, size_t len) {}
} // namespace serial
