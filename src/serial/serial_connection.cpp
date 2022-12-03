#include "serial/serial_connection.hpp"
#include "base85/base85.hpp"
#include <cstring>
#include <iostream>
#include <map>
#include <ostream>

namespace serial {
static const char PACKET_IDS[]{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                               'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                               'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
                               'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '!', '#', '$', '%', '&', '(',
                               ')', '*', '+', '-', ';', '<', '=', '>', '?', '@', '^', '_', '`', '{', '|', '}', '~'};
static std::map<const char, const uint8_t> *generate_char_map();
static std::map<const char, const uint8_t> *BUF_TO_CHAR = generate_char_map();

SerialConnection &SerialConnection::get_instance() {
  static SerialConnection connection = SerialConnection(std::cout.rdbuf(), std::cin.rdbuf());
  return connection;
}

SerialConnection::SerialConnection(std::streambuf *output, std::streambuf *input)
    : output(std::ostream(output)), input(std::istream(input)) {}

PacketData SerialConnection::read_packet(void *ptr, const uint16_t ptr_len) {
  this->input.getline(static_cast<char *>(ptr), ptr_len, '\n');
  uint8_t id = BUF_TO_CHAR->at(static_cast<char *>(ptr)[0]);
  if (static_cast<char *>(ptr)[1] == 0) {
    return {id, nullptr, 0};
  }
  return {id, ptr,
          static_cast<uint16_t>(static_cast<char *>(base85::b85tobin(ptr, static_cast<char *>(ptr) + 1)) -
                                static_cast<char *>(ptr))};
}

void SerialConnection::send_packet(uint8_t packet_id, void *ptr, const uint16_t len) {
  auto charPtr = static_cast<char *>(ptr);
  this->write_prefix();
  this->output.write(&PACKET_IDS[packet_id], 1);
  base85::bintob85(charPtr, ptr, len);
  this->output.write(charPtr, static_cast<std::streamsize>(std::strlen(charPtr)));
}

uint8_t SerialConnection::read_packet_id() { return BUF_TO_CHAR->at(this->input.peek()); }

void SerialConnection::sync_output() { this->output.flush(); }

void SerialConnection::skip_to_end() { this->input.ignore(); }

void SerialConnection::write_prefix() { this->output.write(".[", 2); }

static std::map<const char, const uint8_t> *generate_char_map() {
  auto map = new std::map<const char, const uint8_t>;
  for (uint8_t i = 0; i < 85; i++) {
    map->emplace(PACKET_IDS[i], i);
  }
  return map;
}
} // namespace serial
