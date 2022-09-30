#ifndef SCREEN_HPP
#define SCREEN_HPP
#include <string>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#pragma clang diagnostic pop
#include "robot.hpp"

namespace screen {
#define BASE_HEIGHT static_cast<lv_coord_t>(40)

class Screen {
public:
  virtual void create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) = 0;
  virtual void update(Robot *robot) = 0;
  virtual void initialize(lv_coord_t width, lv_coord_t height) = 0;
  virtual void destroy(lv_obj_t *screen) = 0;
};

void initialize(Robot *robot);
void add_screen(Screen *screen);
}
#endif //SCREEN_HPP
