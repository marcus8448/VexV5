#ifndef SCREEN_HPP
#define SCREEN_HPP
#include <string>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#pragma GCC diagnostic pop
#include "robot/robot.hpp"

namespace screen {
#define BASE_HEIGHT static_cast<lv_coord_t>(40)

class Screen {
public:
  virtual void create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) = 0;
  virtual void update(robot::Robot *robot) = 0;
};

void initialize(robot::Robot *robot);
void add_screen(Screen *screen);
} // namespace screen
#endif // SCREEN_HPP
