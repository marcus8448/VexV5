#include "serial/serial_connection.hpp"
#include "base85/base85.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <istream>
#include <ostream>
#include <map>

namespace serial {
static const char PACKET_IDS[]{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                                 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
                                 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '!', '#', '$', '%', '&', '(',
                                 ')', '*', '+', '-', ';', '<', '=', '>', '?', '@', '^', '_', '`', '{', '|', '}', '~'};
static std::map<const char, const uint8_t> *generate_char_map();
static std::map<const char, const uint8_t> *buf_to_char = generate_char_map();

static void *buffer = malloc(512);

SerialConnection &SerialConnection::get_instance() {
  static SerialConnection connection = SerialConnection(std::cout.rdbuf(), std::cin.rdbuf());
  return connection;
}

SerialConnection::SerialConnection(std::streambuf *output, std::streambuf *input)
    : output(std::ostream(output)), input(std::istream(input)) {}

uint16_t SerialConnection::read_packet(void *ptr, const uint16_t len) {
  auto charPtr = static_cast<char*>(ptr);
  this->input.getline(charPtr, len, '\n');
  return static_cast<char*>(base85::b85tobin(ptr, charPtr)) - charPtr;
}

void SerialConnection::send_packet(uint8_t packet_id, void *ptr, const uint16_t len) {
  auto charPtr = static_cast<char*>(ptr);
  this->write_prefix();
  this->output.write(&PACKET_IDS[packet_id], 1);
  base85::bintob85(charPtr, ptr, len);
  this->output.write(charPtr, std::strlen(charPtr));
  this->write_suffix();
}

uint8_t SerialConnection::read_packet_id() {
  char c = 0;
  this->input.read(&c, 1);
  return buf_to_char->at(c);
}

void SerialConnection::sync_output() {

}

size_t SerialConnection::available() {

}

void SerialConnection::skip_to_end() {
}

static std::map<const char, const uint8_t> *generate_char_map() {
  auto map = new std::map<const char, const uint8_t>;
  for (uint8_t i = 0; i < 85; i++) {
    map->emplace(PACKET_IDS[i], i);
  }
  return map;
}
}
