#ifndef SERIALLINK_DEBUG_HPP
#define SERIALLINK_DEBUG_HPP

#include <streambuf>

namespace serial {
class SerialConnection {
private:
  std::streambuf *output;
  std::streambuf *input;
public:
  SerialConnection(std::streambuf *output, std::streambuf *input);

  void send(const char id[4], void* data, uint16_t len);
  uint16_t read(void* ptr, uint16_t len);
};

class SerialPlugin {
public:
  virtual void initialize() = 0;
  virtual void handle(SerialConnection *connection, void* buffer, size_t len) = 0;
};

void add_plugin(uint16_t id, SerialPlugin *plugin);

void initialize();
} // namespace serial
#endif // SERIALLINK_DEBUG_HPP
