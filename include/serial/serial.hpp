#ifndef SERIAL_DEBUG_HPP
#define SERIAL_DEBUG_HPP

#include <map>
#include <streambuf>

namespace serial {
class SerialPlugin;
class IdRegistry {
private:
  uint16_t size = 0;
  std::map<const uint16_t, const char *> *idToName;
  std::map<const uint16_t, SerialPlugin *> *idToPlugin;
  std::map<const char *, const uint16_t> *nameToId;

public:
  IdRegistry();

  uint16_t register_packet(const char *name, SerialPlugin *plugin);
  const char *get_name(uint16_t id);
  uint16_t get_id(const char *name);
};

class SerialConnection {
private:
  std::streambuf *output;
  std::streambuf *input;
  IdRegistry *registry;

public:
  SerialConnection(std::streambuf *output, std::streambuf *input, IdRegistry *registry);

  void send(const char *name, const void *data, uint16_t len);
  void send_exact(const void *data, uint16_t len);
  void send_exact(uint16_t id);

  void read_exact(void *ptr, uint16_t size);
  void read_null_term(char *ptr, uint16_t size);

  void sync_output();
  size_t available();
  void skip_to_end();

  uint16_t read_variable(void *ptr, uint16_t len);
};

class SerialPlugin {
public:
  virtual void initialize() = 0;
  virtual void handle(SerialConnection *connection, void *buffer, size_t len) = 0;
  virtual void register_packets(IdRegistry *registry) = 0;
};

void add_plugin(const uint16_t id, SerialPlugin *plugin);

void initialize();
} // namespace serial
#endif // SERIAL_DEBUG_HPP
