#include "screen/config.hpp"
#include "configuration.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_btn.h"
#include "display/lv_objx/lv_label.h"
#pragma GCC diagnostic pop

namespace screen {
lv_res_t switch_scheme(lv_obj_t *btn);
void update_label(lv_obj_t *btn);

Configuration::Configuration() = default;

void Configuration::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
  auto halfWidth = static_cast<lv_coord_t>(width / 2);
  lv_obj_t *btn = lv_btn_create(screen, nullptr);
  lv_obj_set_pos(btn, 0, 48 * 0);
  lv_obj_set_size(btn, halfWidth, 48);
  lv_btn_set_toggle(btn, true);
  lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, switch_scheme);
  update_label(btn);
}

void Configuration::initialize(lv_coord_t width, lv_coord_t height) {}

void Configuration::update(robot::Robot *robot) {}

void Configuration::destroy(lv_obj_t *screen) {}

lv_res_t switch_scheme(lv_obj_t *btn) {
  config::set_drivetrain_control_scheme(
      config::get_drivetrain_control_scheme() == config::TANK_DRIVE ? config::ARCADE_DRIVE : config::TANK_DRIVE);
  update_label(btn);
  return LV_RES_OK;
}

void update_label(lv_obj_t *btn) {
  switch (config::get_drivetrain_control_scheme()) {
  case config::ARCADE_DRIVE:
    lv_label_set_text(btn, "Control Scheme: Arcade");
    break;
  case config::TANK_DRIVE:
    lv_label_set_text(btn, "Control Scheme: Tank");
    break;
  }
}
} // namespace screen
