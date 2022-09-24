#ifndef SCREEN_HPP
#define SCREEN_HPP
#include <string>
#include "robot.hpp"

namespace screen {

enum Colour {
  WHITE = 0xFFFFFF,
  RED = 0xFF0000,
  YELLOW = 0xFFFF00,
  GREEN = 0x00FF00,
  BLUE = 0x0000FF
};

void initialize(Robot *robot);
void write_line(const char *string, Colour colour = Colour::WHITE);
void write_line(const std::string &string, Colour colour = Colour::WHITE);

}
#endif //SCREEN_HPP
