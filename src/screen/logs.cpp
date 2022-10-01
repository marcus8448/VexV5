#include "screen/screen.hpp"
#include "screen/lvgl_util.hpp"
#include "screen/logs.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_list.h"
#pragma GCC diagnostic pop

namespace screen {
lv_res_t drop_log(lv_obj_t *obj);

Logging::Logging() {
  screen::logging = this;
}

void Logging::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
  logs = lv_list_create(screen, nullptr);
  lv_obj_set_pos(logs, 0, 0);
  lv_obj_set_size(logs, width, static_cast<lv_coord_t>(height - BASE_HEIGHT));
}

void Logging::initialize(lv_coord_t width, lv_coord_t height) {
}

void Logging::update(robot::Robot *robot) {
}

void Logging::destroy(lv_obj_t *screen) {
}

void Logging::write_line(const char *string, const lv_color_t colour) {
  if (logs != nullptr) {
    if (lv_list_get_size(logs) >= 12) {
      lv_list_remove(logs, 0);
    }

    lv_obj_t *label = lv_list_add(logs, nullptr, string, drop_log);
    lv_obj_set_style(label, create_text_color_style(colour));
  }
}

void Logging::write_line(const std::string &string, const lv_color_t colour) {
  if (logs != nullptr) {
    write_line(string.c_str(), colour);
  }
}

lv_res_t drop_log(lv_obj_t *obj) {
  delete lv_obj_get_style(obj); // safe as we create a new style for each button
  return LV_RES_INV;
}
} // screen
