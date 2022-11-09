#include "screen/banner.hpp"
#include "screen/colour.hpp"
#include "screen/lvgl_util.hpp"
#include "screen/screen.hpp"
#include <cmath>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_canvas.h"
#pragma GCC diagnostic pop

namespace screen {
Banner::Banner() = default;

void Banner::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
  this->imageCanvas = lv_canvas_create(lv_layer_sys(), nullptr);
  lv_obj_set_pos(this->imageCanvas, 0, 0);
  lv_obj_set_size(this->imageCanvas, width, height);
  lv_obj_set_top(this->imageCanvas, true);
  lv_obj_set_hidden(this->imageCanvas, true);
}

void Banner::update(robot::Robot &robot) {}
} // namespace screen
