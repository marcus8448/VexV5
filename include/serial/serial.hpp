#ifndef SERIALLINK_DEBUG_HPP
#define SERIALLINK_DEBUG_HPP

#include <streambuf>
#include <map>

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

  const uint16_t register_packet(const char *name, SerialPlugin *plugin);
  const char* get_name(const uint16_t id);
  const uint16_t get_id(const char* name);
};

class SerialConnection {
private:
  std::streambuf *output;
  std::streambuf *input;
  IdRegistry *registry;
public:
  SerialConnection(std::streambuf *output, std::streambuf *input, IdRegistry *registry);

  void send(const char* name, const void* data, const uint16_t len);
  void send_exact(const void* data, const uint16_t len);
  void send_exact(const uint16_t id);

  void read_exact(void *ptr, const uint16_t size);
  void read_null_term(char *ptr, const uint16_t size);

  void sync_output();
  int16_t available();
  void skip_to_end();

  uint16_t read_variable(void* ptr, const uint16_t len);
};

class SerialPlugin {
public:
  virtual void initialize() = 0;
  virtual void handle(SerialConnection *connection, void* buffer, size_t len) = 0;
  virtual void register_packets(IdRegistry *registry) = 0;
};

void add_plugin(uint16_t id, SerialPlugin *plugin);

void initialize();
} // namespace serial
#endif // SERIALLINK_DEBUG_HPP
