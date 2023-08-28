#include "screen/config_screen.hpp"
#include "debug/logger.hpp"
#include "format.hpp"

namespace screen {
static void switch_drive_scheme([[maybe_unused]] lv_event_t *btn);

ConfigurationScreen::ConfigurationScreen(robot::Robot &robot) : robot(robot) {}

void ConfigurationScreen::initialize(lv_obj_t *screen) {
  this->driveSchemeBtn = lv_btn_create(screen);
  this->driveSchemeBtnLbl = lv_label_create(this->driveSchemeBtn);
  lv_obj_set_pos(this->driveSchemeBtn, 0, 0);
  lv_obj_set_size(this->driveSchemeBtn, SCREEN_HALF_WIDTH, 48);
  lv_obj_add_event_cb(this->driveSchemeBtn, switch_drive_scheme, LV_EVENT_CLICKED, this);
  this->update_drive_scheme_label();
}

void ConfigurationScreen::update() {}

void ConfigurationScreen::update_drive_scheme_label() {
  lv_label_set_text(
      this->driveSchemeBtnLbl,
      fmt::string_format("Control Scheme: %s", robot::driveSchemeName(this->robot.drivetrain.controlScheme)).c_str());
}

void ConfigurationScreen::cleanup() {
  lv_obj_del_async(this->driveSchemeBtn);

  this->driveSchemeBtn = nullptr;
  this->driveSchemeBtnLbl = nullptr;
}

static void switch_drive_scheme(lv_event_t *event) {
  ConfigurationScreen *instance = static_cast<ConfigurationScreen *>(event->user_data);
  instance->robot.drivetrain.controlScheme =
      instance->robot.drivetrain.controlScheme == robot::Drivetrain::ControlScheme::TANK
          ? robot::Drivetrain::ControlScheme::ARCADE
          : robot::Drivetrain::ControlScheme::TANK;
  instance->update_drive_scheme_label();
}
} // namespace screen
