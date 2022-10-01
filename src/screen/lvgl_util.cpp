#include "screen/lvgl_util.hpp"

namespace screen {
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
  auto *style = new lv_style_t{};
  lv_style_copy(style, &lv_style_plain_color);
  style->text.color = colour;
  return style;
}
} // namespace screen