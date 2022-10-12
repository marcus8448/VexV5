#include "screen/lvgl_util.hpp"
#include "display/lv_core/lv_obj.h"
#include "display/lv_core/lv_style.h"

namespace screen {
bool *enableCanvas = new bool;
void *canvasBuffer = nullptr;

lv_obj_t *create_label(lv_obj_t *screen, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, const char *text,
                       lv_style_t *style) {
  auto label = lv_label_create(screen, nullptr);
  if (style != nullptr) {
    lv_obj_set_style(label, style);
  }
  lv_obj_set_pos(label, x, y);
  lv_obj_set_size(label, w, h);
  lv_label_set_text(label, text);
  return label;
}

lv_style_t *create_text_color_style(lv_color_t colour) {
  static lv_style_t *base_style = lv_obj_get_style(lv_scr_act());
  auto *style = new lv_style_t{};
  lv_style_copy(style, base_style);
  style->text.color = colour;
  return style;
}
} // namespace screen