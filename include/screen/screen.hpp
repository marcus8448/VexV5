#ifndef SCREEN_HPP
#define SCREEN_HPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion"
#pragma GCC diagnostic ignored "-Wdeprecated-anon-enum-enum-conversion"
#include "liblvgl/lvgl.h"
#pragma GCC diagnostic pop
#include <string>

#define SCREEN_UPDATE_RATE 50

#define SCREEN_WIDTH static_cast<lv_coord_t>(480)
#define SCREEN_HALF_WIDTH static_cast<lv_coord_t>(240)
#define SCREEN_HEIGHT static_cast<lv_coord_t>(240)

#define BUTTON_SIZE static_cast<lv_coord_t>(40)

namespace screen {
class Screen {
public:
  virtual void initialize(lv_obj_t *screen) = 0;

  virtual void update() = 0;

  virtual void cleanup() = 0;
};

void initialize();
void addScreen(Screen *screen);
void removeScreen(Screen *screen);
} // namespace screen
#endif // SCREEN_HPP
