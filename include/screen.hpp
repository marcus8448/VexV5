#ifndef SCREEN_HPP
#define SCREEN_HPP
#include <string>
#include "robot.hpp"

namespace screen {

enum Colour {
  WHITE = 0xFFFFFF,
  BLACK = 0x000000,
  VEX_GREY = 0,
  RED = 0xFF0000,
  ORANGE = 0xFFA600,
  YELLOW = 0xFFFF00,
  GREEN = 0x00FF00,
  DARK_GREEN = 0x39941B,
  LIGHT_BLUE = 0x00DDFF,
  BLUE = 0x0062FF,
  DARK_BLUE = 0x0000FF,
  PINK = 0xFF70E2,
  INDIGO = 0x4B0082,
  VIOLET = 0x8F00FF
};

void initialize(Robot *robot);
void write_line(const char *string, Colour colour = Colour::WHITE);
void write_line(const std::string &string, Colour colour = Colour::WHITE);

}
#endif //SCREEN_HPP
