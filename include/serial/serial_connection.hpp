#ifndef SERIAL_ROBOT_STATE_HPP
#define SERIAL_ROBOT_STATE_HPP

#include <istream>
#include <ostream>
#include <streambuf>

namespace serial {

class SerialConnection {
private:
  std::ostream output;
  std::istream input;

  SerialConnection(std::streambuf *output, std::streambuf *input);

public:
  static SerialConnection &get_instance();
  
  uint16_t read_packet(void *ptr, uint16_t len);
  void send_packet(uint8_t packet_id, void *ptr, const uint16_t len);

  uint8_t read_packet_id();

  void sync_output();
  size_t available();
  void skip_to_end();

private:
  void write_prefix();
  void write_suffix();
};
} // namespace serial
#endif // SERIAL_ROBOT_STATE_HPP
