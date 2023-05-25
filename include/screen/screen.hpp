#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "lvgl_util.hpp"
#include <string>

#define SCREEN_UPDATE_RATE 50

#define SCREEN_WIDTH 480
#define SCREEN_HALF_WIDTH 240
#define SCREEN_HEIGHT 240

#define CANVAS_WIDTH SCREEN_WIDTH
#define CANVAS_HEIGHT SCREEN_HEIGHT
#define CANVAS_COLOUR LV_IMG_CF_TRUE_COLOR
#define CANVAS_SIZE_BYTES ((CANVAS_WIDTH * CANVAS_HEIGHT * lv_img_color_format_get_px_size(CANVAS_COLOUR)) / 8)

#define BUTTON_SIZE static_cast<lv_coord_t>(40)

namespace screen {
class Screen {
public:
  virtual void initialize(lv_obj_t *screen) = 0;
  virtual void update() = 0;
};

void initialize();
void addScreen(Screen *screen);
void removeScreen(Screen *screen);
} // namespace screen
#endif // SCREEN_HPP
