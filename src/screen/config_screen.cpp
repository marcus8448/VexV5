#include "screen/config_screen.hpp"
#include "configuration.hpp"
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
  this->allianceColourBtn = lv_btn_create(screen, nullptr);
  this->allianceColourBtnLbl = lv_label_create(this->allianceColourBtn, nullptr);
  lv_obj_set_pos(this->driveSchemeBtn, 48 * 0, 48 * 0);
  lv_obj_set_pos(this->allianceColourBtn, 48 * 0, 48 * 1);
  lv_obj_set_size(this->driveSchemeBtn, halfWidth, 48);
  lv_obj_set_size(this->allianceColourBtn, halfWidth, 48);
  lv_btn_set_toggle(this->driveSchemeBtn, false);
  lv_btn_set_toggle(this->allianceColourBtn, false);
  lv_btn_set_action(this->driveSchemeBtn, LV_BTN_ACTION_CLICK, switch_drive_scheme);
  lv_btn_set_action(this->allianceColourBtn, LV_BTN_ACTION_CLICK, switch_alliance_colour);
  this->update_drive_scheme_label();
  this->update_alliance_colour_label();
  instance = this;
}

void ConfigurationScreen::update(robot::Robot *robot) {
}

void ConfigurationScreen::update_drive_scheme_label() {
  switch (config::get_instance()->get_drivetrain_control_scheme()) {
  case config::DrivetrainControlScheme::ARCADE_DRIVE:
    lv_label_set_text(this->driveSchemeBtnLbl, "Control Scheme: Arcade");
    break;
  case config::DrivetrainControlScheme::TANK_DRIVE:
    lv_label_set_text(this->driveSchemeBtnLbl, "Control Scheme: Tank");
    break;
  }
}

void ConfigurationScreen::update_alliance_colour_label() {
  if (this->styleSet) {
    lv_style_t *style = lv_obj_get_style(this->allianceColourBtnLbl);
    lv_obj_set_style(this->allianceColourBtnLbl, lv_obj_get_style(lv_scr_act())); // temporary style
    free(style);
  }
  switch (config::get_instance()->get_alliance_colour()) {
  case config::AllianceColour::RED:
    lv_label_set_text(this->allianceColourBtnLbl, "Alliance Colour: Red");
    lv_label_set_style(this->allianceColourBtnLbl, create_text_color_style(colour::RED));
    break;
  case config::AllianceColour::BLUE:
    lv_label_set_text(this->allianceColourBtnLbl, "Alliance Colour: Blue");
    lv_label_set_style(this->allianceColourBtnLbl, create_text_color_style(colour::BLUE));
    break;
  }
  this->styleSet = true;
}

lv_res_t switch_drive_scheme(lv_obj_t *btn) {
  config::get_instance()->set_drivetrain_control_scheme(config::get_instance()->get_drivetrain_control_scheme() ==
                                                                config::DrivetrainControlScheme::TANK_DRIVE
                                                            ? config::DrivetrainControlScheme::ARCADE_DRIVE
                                                            : config::DrivetrainControlScheme::TANK_DRIVE);
  instance->update_drive_scheme_label();
  return LV_RES_INV;
}

lv_res_t switch_alliance_colour(lv_obj_t *btn) {
  config::get_instance()->set_alliance_colour(config::get_instance()->get_alliance_colour() ==
                                                                config::AllianceColour::RED
                                                            ? config::AllianceColour::BLUE
                                                            : config::AllianceColour::RED);
  instance->update_alliance_colour_label();
  return LV_RES_INV;
}
} // namespace screen
