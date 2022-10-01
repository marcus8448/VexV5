#ifndef SCREEN_COLOUR_HPP
#define SCREEN_COLOUR_HPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_misc/lv_color.h"
#pragma GCC diagnostic pop

namespace screen::colour {
static const lv_color_t WHITE = lv_color_hex(0xFFFFFF);
static const lv_color_t BLACK = lv_color_hex(0x000000);
static const lv_color_t VEX_GREY = lv_color_hex(0x000000); // todo
static const lv_color_t RED = lv_color_hex(0xFF0000);
static const lv_color_t ORANGE = lv_color_hex(0xFFA600);
static const lv_color_t YELLOW = lv_color_hex(0xFFFF00);
static const lv_color_t GREEN = lv_color_hex(0x00FF00);
static const lv_color_t DARK_GREEN = lv_color_hex(0x39941B);
static const lv_color_t LIGHT_BLUE = lv_color_hex(0x00DDFF);
static const lv_color_t BLUE = lv_color_hex(0x0062FF);
static const lv_color_t DARK_BLUE = lv_color_hex(0x0000FF);
static const lv_color_t PINK = lv_color_hex(0xFF70E2);
static const lv_color_t INDIGO = lv_color_hex(0x4B0082);
static const lv_color_t VIOLET = lv_color_hex(0x8F00FF);
} // namespace screen::colour
#endif // SCREEN_COLOUR_HPP
