#ifndef SCREEN_LVGL_UTIL_HPP
#define SCREEN_LVGL_UTIL_HPP

#include "debug/logger.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#include "display/lv_core/lv_style.h"
#include "display/lv_misc/lv_color.h"
#include "display/lv_objx/lv_btn.h"
#include "display/lv_objx/lv_canvas.h"
#include "display/lv_objx/lv_label.h"
#include "display/lv_objx/lv_list.h"
#pragma GCC diagnostic pop
#include <cstdlib>

namespace screen {
lv_obj_t *create_canvas(lv_obj_t *screen);

lv_obj_t *create_label(lv_obj_t *screen, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h,
                       const char *text = "<uninitialized>", lv_style_t *style = nullptr);

lv_style_t *get_base_style();
lv_style_t *create_style();
lv_style_t *create_text_color_style(lv_color_t colour);

lv_obj_t *create_info_label(lv_obj_t *screen, bool right, lv_coord_t index, const char *text = "<uninitialized>");

template <typename... Args> void set_label_text(lv_obj_t *label, const char *format, Args... args) {
  static char *buffer = new char[64];
  snprintf(buffer, 64, format, args...);
  lv_label_set_text(label, buffer);
}
} // namespace screen

#endif // SCREEN_LVGL_UTIL_HPP
