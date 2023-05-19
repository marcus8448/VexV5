#ifndef SERIAL_CONNECTION_HPP
#define SERIAL_CONNECTION_HPP

#include <istream>
#include <ostream>
#include <streambuf>

namespace serial {
struct PacketData {
  uint8_t id;
  void *data;
  uint16_t len;
};

class SerialConnection {
private:
  std::ostream output;
  std::istream input;

  SerialConnection(std::streambuf *output, std::streambuf *input);

public:
  static SerialConnection &get_instance();

  PacketData read_packet(void *ptr, uint16_t ptr_len);
  void send_packet(uint8_t packet_id, void *ptr, uint16_t len);

  uint8_t read_packet_id();

  void sync_output();
  void skip_to_end();

private:
  void write_prefix();
};
} // namespace serial
#endif // SERIAL_CONNECTION_HPP
