#ifndef SCREEN_HPP
#define SCREEN_HPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion"
#pragma GCC diagnostic ignored "-Wdeprecated-anon-enum-enum-conversion"
#include "liblvgl/lvgl.h"
#include "robot/robot.hpp"
#pragma GCC diagnostic pop
#include <functional>
#include <string>

#define SCREEN_UPDATE_RATE 50

#define SCREEN_WIDTH static_cast<lv_coord_t>(480)
#define SCREEN_HALF_WIDTH static_cast<lv_coord_t>(240)
#define SCREEN_HEIGHT static_cast<lv_coord_t>(240)

#define BUTTON_SIZE static_cast<lv_coord_t>(40)

#define SCREEN_CB_ADV(TYPE, NAME)                                                                                      \
  static void NAME(lv_event_t *event) {                                                                                \
    auto inst = static_cast<TYPE *>(event->user_data);                                                                 \
    inst->NAME(event);                                                                                                 \
  }
#define SCREEN_CB(TYPE, NAME)                                                                                          \
  static void NAME(lv_event_t *event) {                                                                                \
    auto inst = static_cast<TYPE *>(event->user_data);                                                                 \
    inst->NAME();                                                                                                      \
  }
#define SCREEN_ADD(TYPE)                                                                                               \
  screen::addScreen([](robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {                   \
    return new screen::TYPE(robot, screen, width, height);                                                             \
  })

namespace screen {
class Screen {
public:
  const lv_coord_t width;
  const lv_coord_t height;
  robot::Robot &robot;

  explicit Screen(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height);
  virtual ~Screen() = 0;

  virtual void update() = 0;
};

void initialize(robot::Robot &robot);
void addScreen(
    const std::function<Screen *(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height)> &screen);
void removeScreen(
    const std::function<Screen *(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width, lv_coord_t height)> &screen);
} // namespace screen
#endif // SCREEN_HPP
