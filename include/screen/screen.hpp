#ifndef SCREEN_HPP
#define SCREEN_HPP
#include <string>
#include "robot.hpp"

namespace screen {
#define BASE_HEIGHT static_cast<lv_coord_t>(40)

enum Colour {
  WHITE = 0xFFFFFF,
  BLACK = 0x000000,
  VEX_GREY = 0x000000, //fixme
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

void add_screen(
    void (*createFun)(lv_obj_t *screen, lv_coord_t width, lv_coord_t height),
    void (*updateFun)(Robot *robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height),
    void (*initFun)(lv_obj_t *screen, lv_coord_t width, lv_coord_t height),
    void (*dropFun)(lv_obj_t *screen)
);

}
#endif //SCREEN_HPP
