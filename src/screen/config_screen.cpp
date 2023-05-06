#include "screen/config_screen.hpp"
#include "configuration.hpp"
#include "debug/logger.hpp"
#include "screen/colour.hpp"
#include "screen/lvgl_util.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_btn.h"
#include "display/lv_objx/lv_label.h"
#pragma GCC diagnostic pop

namespace screen {
static ConfigurationScreen *instance = nullptr;
ConfigurationScreen::ConfigurationScreen() = default;

void ConfigurationScreen::create(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
  auto halfWidth = static_cast<lv_coord_t>(width / 2);
  this->driveSchemeBtn = lv_btn_create(screen, nullptr);
  this->driveSchemeBtnLbl = lv_label_create(this->driveSchemeBtn, nullptr);
  lv_obj_set_pos(this->driveSchemeBtn, 48 * 0, 48 * 0);
  lv_obj_set_size(this->driveSchemeBtn, halfWidth, 48);
  lv_btn_set_toggle(this->driveSchemeBtn, false);
  lv_btn_set_action(this->driveSchemeBtn, LV_BTN_ACTION_CLICK, switch_drive_scheme);
  this->update_drive_scheme_label();
  instance = this;
}

void ConfigurationScreen::update(robot::Robot &robot) {}

void ConfigurationScreen::update_drive_scheme_label() {
  lv_label_set_text(
      this->driveSchemeBtnLbl,
      logger::string_format("Control Scheme: %s", config::get_drive_scheme_name(config::controlScheme)).c_str());
}

lv_res_t switch_drive_scheme([[maybe_unused]] lv_obj_t *btn) {
  config::controlScheme = config::controlScheme == config::DrivetrainControlScheme::TANK
                              ? config::DrivetrainControlScheme::ARCADE
                              : config::DrivetrainControlScheme::TANK;
  instance->update_drive_scheme_label();
  return LV_RES_INV;
}
} // namespace screen
