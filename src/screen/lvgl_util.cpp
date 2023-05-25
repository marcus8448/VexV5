#include "screen/lvgl_util.hpp"
#include "screen/screen.hpp"

namespace screen {
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
  lv_style_t *style = create_style();
  style->text.color = colour;
  return style;
}

lv_obj_t *create_info_label(lv_obj_t *screen, bool right, lv_coord_t index, const char *text) {
  return create_label(screen, right ? SCREEN_HALF_WIDTH : static_cast<lv_coord_t>(0), static_cast<lv_coord_t>(index * 16),
                      SCREEN_HALF_WIDTH, 16, text);
}

lv_obj_t *create_canvas(lv_obj_t *screen) {
  if (canvasBuffer == nullptr) {
    canvasBuffer = calloc(CANVAS_SIZE_BYTES, 1);
  }

  lv_obj_t* canvas = lv_canvas_create(screen, nullptr);
  lv_obj_set_pos(canvas, 0, 0);
  lv_obj_set_size(canvas, CANVAS_WIDTH, CANVAS_HEIGHT);
  lv_canvas_set_buffer(canvas, canvasBuffer, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_COLOUR);
  return canvas;
}

lv_style_t *get_base_style() {
  return lv_obj_get_style(lv_scr_act());
}

lv_style_t *create_style() {
  auto *style = static_cast<lv_style_t *>(malloc(sizeof(lv_style_t)));
  lv_style_copy(style, get_base_style());
  return style;
}
} // namespace screen
