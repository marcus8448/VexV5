#ifndef SCREEN_SCREEN_HPP
#define SCREEN_SCREEN_HPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion"
#pragma GCC diagnostic ignored "-Wdeprecated-anon-enum-enum-conversion"
#include "liblvgl/lvgl.h"
#include "robot/robot.hpp"
#pragma GCC diagnostic pop
#include <functional>

#define SCREEN_CB_ADV(TYPE, NAME) [](lv_event_t *event) { static_cast<TYPE *>(event->user_data)->NAME(event); }
#define SCREEN_CB(TYPE, NAME) [](lv_event_t *event) { static_cast<TYPE *>(event->user_data)->NAME(); }

namespace screen {
constexpr lv_coord_t BUTTON_SIZE = 40;
constexpr uint32_t UPDATE_RATE = 50;

constexpr lv_coord_t coord(const int coord) { return static_cast<lv_coord_t>(coord); }

struct LvObjDeleter {
  void operator()(lv_obj_t *obj) const;
};

typedef std::unique_ptr<lv_obj_t, LvObjDeleter> lv_obj;

class Screen {
public:
  const lv_coord_t width;
  const lv_coord_t height;
  robot::Robot &robot;

  explicit Screen(robot::Robot &robot, lv_coord_t width, lv_coord_t height);
  Screen(const Screen &) = delete;
  Screen &operator=(const Screen &) = delete;

  virtual ~Screen() = 0;

  virtual void update() = 0;
};

void initialize(robot::Robot &robot);
void addScreen(const std::function<std::unique_ptr<Screen>(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width,
                                                           lv_coord_t height)> &screen);
void removeScreen(const std::function<std::unique_ptr<Screen>(robot::Robot &robot, lv_obj_t *screen, lv_coord_t width,
                                                              lv_coord_t height)> &screen);
} // namespace screen
#endif // SCREEN_SCREEN_HPP
