#include "screen/config_screen.hpp"
#include "debug/logger.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include "display/lv_objx/lv_btn.h"
#include "display/lv_objx/lv_label.h"
#pragma GCC diagnostic pop

namespace screen {
static ConfigurationScreen *instance = nullptr;
ConfigurationScreen::ConfigurationScreen(robot::Robot &robot) : robot(robot) {}

void ConfigurationScreen::initialize(lv_obj_t *screen) {
  this->driveSchemeBtn = lv_btn_create(screen, nullptr);
  this->driveSchemeBtnLbl = lv_label_create(this->driveSchemeBtn, nullptr);
  lv_obj_set_pos(this->driveSchemeBtn, 0, 0);
  lv_obj_set_size(this->driveSchemeBtn, SCREEN_HALF_WIDTH, 48);
  lv_btn_set_toggle(this->driveSchemeBtn, false);
  lv_btn_set_action(this->driveSchemeBtn, LV_BTN_ACTION_CLICK, switch_drive_scheme);
  this->update_drive_scheme_label();
  instance = this;
}

void ConfigurationScreen::update() {}

void ConfigurationScreen::update_drive_scheme_label() {
  lv_label_set_text(
      this->driveSchemeBtnLbl,
      logger::string_format("Control Scheme: %s", robot::driveSchemeName(this->robot.drivetrain.controlScheme))
          .c_str());
}

lv_res_t switch_drive_scheme([[maybe_unused]] lv_obj_t *btn) {
  instance->robot.drivetrain.controlScheme =
      instance->robot.drivetrain.controlScheme == robot::Drivetrain::ControlScheme::TANK
          ? robot::Drivetrain::ControlScheme::ARCADE
          : robot::Drivetrain::ControlScheme::TANK;
  instance->update_drive_scheme_label();
  return LV_RES_INV;
}
} // namespace screen
