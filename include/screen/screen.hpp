#ifndef SCREEN_HPP
#define SCREEN_HPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#pragma GCC diagnostic pop
#include <string>

#define SCREEN_UPDATE_RATE 50

namespace screen {
#define BASE_HEIGHT static_cast<lv_coord_t>(40)

class Screen {
public:
  virtual void initialize(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) = 0;
  virtual void update() = 0;
};

void initialize();
void addScreen(Screen *screen);
void removeScreen(Screen *screen);
} // namespace screen
#endif // SCREEN_HPP
